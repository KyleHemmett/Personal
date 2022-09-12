import ssl
import paho.mqtt.client as mqtt
from time import sleep

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    client.connected_flag=True
    client.subscribe("team26/downlink")

    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client(transport="websockets")
client.on_connect = on_connect
client.on_message = on_message
username = "team26"
password = "e5aabb21296d9254786e9aa0"
client.tls_set(cert_reqs=ssl.CERT_NONE)
client.ws_set_options(path="/ws")
client.username_pw_set(username, password)


client.connect("tp-mqtt.uqcloud.net", 443, 60)


# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
# client.loop_start()

# count = 0
# while True:
#     sat = client.publish("team26/downlink", count)
#     count = count + 1
#     sleep(.1)

