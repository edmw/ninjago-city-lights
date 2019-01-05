///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Michael Baumgärtner
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

#if OS_USE_NETWORK
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// #include "uMQTTBroker.h"
#endif

#define REALM F("Driver")

#include <OS.h>
extern OS os;

#include "Driver.h"
#include "private.h"

Driver driver;

///////////////////////////////////////////////////////////////////////////////////////////////////
#if OS_USE_NETWORK

#define REALM_MQTT F("MQTT")

const char* mqtt_server = "192.168.178.111";

WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);

static const String mqtt_control = String("control/");
static const String mqtt_status = String("status/");
static const String mqtt_status_self = mqtt_status + "Self";
static const String mqtt_subscribe_pattern = mqtt_control + "#";

bool mqtt_reconnect = false;
elapsed_millis mqtt_reconnect_timeout;

bool mqtt_connect() {
    if (pubSubClient.connected()) return true;

    if (mqtt_reconnect && mqtt_reconnect_timeout < 5000) return false;

    os.notification.info(REALM_MQTT, F("Connecting ..."));
    String clientId = os.network.deviceid + "-" + String(random(0xffff), HEX);
    bool connected = pubSubClient.connect(
        clientId.c_str(),
        mqtt_status_self.c_str(), 0, 1, "disconnected"
    );
    if (connected) {
        os.notification.info(REALM_MQTT, F("Connected"));
        mqtt_reconnect = false;
        return true;
    }
    else {
        os.notification.info(REALM_MQTT, F("Connection failed: "), pubSubClient.state());
        mqtt_reconnect = true;
        mqtt_reconnect_timeout = 0;
        return false;
    }
}

bool mqtt_subscribe() {
    if (pubSubClient.connected()) {
        pubSubClient.subscribe(mqtt_subscribe_pattern.c_str());
        return true;
    }
    return false;
}

void mqtt_self(String data) {
    String topic = mqtt_status_self;
    os.notification.info(REALM_MQTT, F("Publish:"), topic + ":" + data);
    pubSubClient.publish(topic.c_str(), data.c_str(), true);
    delay(10);
}

void mqtt_publish(PWMLED *led) {
    String topic = mqtt_status + led->label;
    String data = (led->is_active()) ? "true" : "false";
    os.notification.info(REALM_MQTT, F("Publish:"), topic + ":" + data);
    pubSubClient.publish(topic.c_str(), data.c_str());
    delay(10);
}

void mqtt_on_data(char* topic_str, byte* payload, unsigned int length) {
    String topic = String(topic_str);

    char data_str[length + 1];
    os_memcpy(data_str, payload, length);
    data_str[length] = '\0';
    String value = (String)data_str;

    if (topic.startsWith(mqtt_control)) {
        String label = topic.substring(mqtt_control.length());
        auto it = driver.leds.find(label);
        if (it != driver.leds.end()) {
            PWMLED* led = it->second;
            if (value == "true") {
                led->activate();
                os.notification.info(REALM_MQTT, F("activate"), label);
                mqtt_publish(led);
            }
            else if (value == "false") {
                led->deactivate();
                os.notification.info(REALM_MQTT, F("deactivate"), label);
                mqtt_publish(led);
            }
            else {
                os.notification.info(REALM_MQTT, F("received data"), value);
            }
        }
        else {
            os.notification.info(REALM_MQTT, F("received for label"), label);
        }
    }
    else {
        os.notification.info(REALM_MQTT, F("received on topic"), topic);
    }
}

#endif
///////////////////////////////////////////////////////////////////////////////////////////////////

void add_led(PWMLED& led) {
    delay(10);
    driver.leds[led.label] = &led;
}

void setup() {
    os.begin(PRODUCTION);

    delay(10);

    driver.tlc.THE_TLC_BEGIN();
    os.notification.info(REALM, F("TLC begin successful"));

    // root
    #ifdef TLC_ROOT
    driver.root_blinker.begin(500, 500);
    add_led(driver.root_blinker);
    driver.root_lighter.begin(1.0, true);
    add_led(driver.root_lighter);
    #endif

    // configure obstacle light:
    // @see http://www.avaids.com/icao.pdf
    // Medium-intensity, Type B
    // color: red, flashing: 20-60 fpm
    #ifdef TLC_TOWER
    driver.obstacle_light.begin(100, 1900);
    driver.obstacle_light.set_dim(0.5);
    add_led(driver.obstacle_light);
    #endif

    // configure tower lights:
    // blue lights on full brightness
    #ifdef TLC_TOWER
    driver.tower_light_1.TLC_TOWER_LIGHT_BEGIN; add_led(driver.tower_light_1);
    driver.tower_light_2.TLC_TOWER_LIGHT_BEGIN; add_led(driver.tower_light_2);
    #endif

    // configure tree lights:
    #ifdef TLC_TOWER
    driver.tree_1.begin(0.5, true); add_led(driver.tree_1);
    driver.tree_2.begin(0.5, true); add_led(driver.tree_2);
    #endif

    // roof: configure bar
    #ifdef TLC_ROOF
    driver.bar_light_1.begin(0.3, true); add_led(driver.bar_light_1);
    driver.bar_light_2.begin(0.3, true); add_led(driver.bar_light_2);
    driver.bar_light_3.begin(0.3, true); add_led(driver.bar_light_3);
    driver.bar_light_4.begin(0.3, true); add_led(driver.bar_light_4);
    driver.bar_light_5.begin(0.3, true); add_led(driver.bar_light_5);
    driver.bar_light_6.begin(0.3, true); add_led(driver.bar_light_6);
    #endif

    // roof: configure store
    #ifdef TLC_ROOF
    driver.store_light_1.begin(1.0, true); add_led(driver.store_light_1);
    driver.store_light_2.begin(1.0, true); add_led(driver.store_light_2);
    driver.store_light_3.begin(1.0, true); add_led(driver.store_light_3);
    driver.store_light_4.begin(1.0, true); add_led(driver.store_light_4);
    #endif

    // upper street level: configure decoration lights
    #ifdef TLC_GALLERY_EAST
    driver.decoration_light_1.begin(0.2, true); add_led(driver.decoration_light_1);
    #endif
    #ifdef TLC_GALLERY_SOUTH
    driver.decoration_light_2.begin(0.2, true); add_led(driver.decoration_light_2);
    #endif

    // upper street level: configure gallery lights
    #ifdef TLC_GALLERY_EAST
    driver.gallery_light_7.begin(0.2); add_led(driver.gallery_light_7);
    driver.gallery_light_8.begin(0.2); add_led(driver.gallery_light_8);
    #endif
    #ifdef TLC_GALLERY_SOUTH
    driver.gallery_light_9.begin(0.2); add_led(driver.gallery_light_9);
    driver.gallery_light_0.begin(0.2); add_led(driver.gallery_light_0);
    #endif

    // configure restaurant
    #ifdef TLC_GALLERY_SOUTH
    driver.restaurant_light.begin(1.0, true); add_led(driver.restaurant_light);
    #endif
    // configure shop
    #ifdef TLC_GALLERY_SOUTH
    driver.shop_light.begin(1.0, true); add_led(driver.shop_light);
    #endif

    // lower street level: configure gallery lights:
    #ifdef TLC_GALLERY_EAST
    driver.gallery_light_1.begin(0.2); add_led(driver.gallery_light_1);
    driver.gallery_light_2.begin(0.2); add_led(driver.gallery_light_2);
    driver.gallery_light_3.begin(0.2); add_led(driver.gallery_light_3);
    #endif
    #ifdef TLC_GALLERY_SOUTH
    driver.gallery_light_4.begin(0.2); add_led(driver.gallery_light_4);
    driver.gallery_light_5.begin(0.2); add_led(driver.gallery_light_5);
    driver.gallery_light_6.begin(0.2); add_led(driver.gallery_light_6);
    #endif

    // lower street level: configure billboard lights
    #ifdef TLC_GALLERY_SOUTH
    driver.billboard_light_1.begin(1000, 1000, true);
    driver.billboard_light_2.begin(1000, 1000, false);
    driver.billboard_light_1.set_dim(0.1);
    driver.billboard_light_2.set_dim(0.1);
    add_led(driver.billboard_light_2);
    add_led(driver.billboard_light_1);
    #endif

    // configure tea room
    #ifdef TLC_GALLERY_EAST
    driver.tea_room.begin(0.5, true); add_led(driver.tea_room);
    #endif
    // configure bed room
    #ifdef TLC_GALLERY_SOUTH
    driver.bed_room.begin(0.5, true); add_led(driver.bed_room);
    #endif
    // configure bath room
    #ifdef TLC_TOWER
    driver.bath_room.begin(1.0, true); add_led(driver.bath_room);
    #endif

    // configure bridge
    #ifdef TLC_GROUND
    driver.bridge_light_1.begin(1.0, true); add_led(driver.bridge_light_1);
    driver.bridge_light_2.begin(1.0, true); add_led(driver.bridge_light_2);
    #endif

    // configure marketstall
    #ifdef TLC_GROUND
    driver.marketstall_light.begin(1.0, true); add_led(driver.marketstall_light);
    #endif

    // configure phone
    #ifdef TLC_GROUND
    driver.phone_light.begin(10);
    driver.phone_light.set_dim(0.02);
    add_led(driver.phone_light);
    #endif

    // configure ground lanterns
    #ifdef TLC_GROUND
    driver.lantern_light_1.begin(0.2); add_led(driver.lantern_light_1);
    driver.lantern_light_2.begin(0.2); add_led(driver.lantern_light_2);
    driver.lantern_light_3.begin(0.2); add_led(driver.lantern_light_3);
    #endif

    delay(10);

    driver.tlc.THE_TLC_WRITE();
    os.notification.info(REALM, F("TLC write successful"));

    #if OS_USE_NETWORK
    delay(10);

    if (os.network.connect()) {
        delay(10);

        os.notification.info(REALM, F("Connect to network successful"), WiFi.localIP());
        WiFi.setAutoReconnect(true);

        pubSubClient.setServer(mqtt_server, 1883);
        pubSubClient.setCallback(mqtt_on_data);
    }
    else {
        os.notification.fatal(REALM, F("Failed: connect to network"));
    }
    #endif

    #if OS_USE_NETWORK
    if (mqtt_connect()) {
        mqtt_self("connected");
        for (auto const& it: driver.leds) {
            PWMLED* led = it.second;
            led->deactivate();
            mqtt_publish(led);
        }
        mqtt_subscribe();
    }
    #endif

    os.notification.info(F("LEGO Ninjago City Lights"), F("Setup complete!"));
}

void loop() {
    bool update = false;

    for (auto const& it: driver.leds) {
        PWMLED* led = it.second;
        update |= led->loop();
    }

    if (update) {
        driver.tlc.THE_TLC_WRITE();
    }

    #if OS_USE_NETWORK
    if (pubSubClient.connected()) {
        pubSubClient.loop();
    }
    else {
        if (mqtt_connect()) {
            mqtt_self("connected");
            mqtt_subscribe();
        }
    }
    #endif

    os.yield();
}
