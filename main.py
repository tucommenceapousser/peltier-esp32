import network
import socket
from machine import Pin, PWM
import onewire, ds18x20, time
import ure

# === CONFIGURATION WIFI ===
SSID = "TonSSID"
PASSWORD = "TonMotDePasse"

wifi = network.WLAN(network.STA_IF)
wifi.active(True)
wifi.connect(SSID, PASSWORD)
while not wifi.isconnected():
    time.sleep(1)
print("Connecté au Wi-Fi, IP:", wifi.ifconfig()[0])

# === CONFIGURATION PELTIER ===
PWM_PIN = 12
peltier = PWM(Pin(PWM_PIN), freq=1000)
peltier.duty(0)  # PWM initial

# === CAPTEUR DS18B20 ===
ds_pin = Pin(13)
ow = onewire.OneWire(ds_pin)
ds = ds18x20.DS18X20(ow)
roms = ds.scan()
print("Capteurs DS18B20 trouvés:", roms)

def read_temp():
    ds.convert_temp()
    time.sleep_ms(750)
    return ds.read_temp(roms[0])

# === SERVEUR WEB ===
def web_page(temp, pwm_value):
    html = f"""<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="5">
<title>ESP32 Peltier Dashboard</title>
<style>
body {{ font-family: Arial; background-color: #111; color: #0f0; text-align:center; }}
h1 {{ color: #0ff; }}
input[type=range] {{ width: 50%; }}
button {{ padding:10px; margin:5px; background-color:#222; color:#0f0; border:1px solid #0f0; cursor:pointer; }}
</style>
</head>
<body>
<h1>ESP32 Peltier Controller</h1>
<p>Température: {temp:.2f} °C</p>
<p>Puissance Peltier: {pwm_value}/1023</p>

<form action="/" method="get">
    <input type="range" name="pwm" min="0" max="1023" value="{pwm_value}" onchange="this.form.submit()">
    <br>
    <button type="submit" name="action" value="up">+</button>
    <button type="submit" name="action" value="down">-</button>
</form>
</body>
</html>
"""
    return html

# === SOCKET SERVEUR ===
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)
print("Serveur web démarré sur port 80")

pwm_value = 0

while True:
    try:
        conn, addr = s.accept()
        request = conn.recv(1024)
        request_str = str(request)
        
        # Lire valeur PWM depuis l'URL
        pwm_match = ure.search(r'pwm=(\d+)', request_str)
        action_match = ure.search(r'action=(\w+)', request_str)
        
        if pwm_match:
            pwm_value = int(pwm_match.group(1))
        elif action_match:
            if action_match.group(1) == 'up':
                pwm_value = min(pwm_value + 50, 1023)
            elif action_match.group(1) == 'down':
                pwm_value = max(pwm_value - 50, 0)
        
        peltier.duty(pwm_value)
        temp = read_temp()
        response = web_page(temp, pwm_value)
        
        conn.send('HTTP/1.1 200 OK\n')
        conn.send('Content-Type: text/html\n')
        conn.send('Connection: close\n\n')
        conn.sendall(response)
        conn.close()
    except Exception as e:
        print("Erreur:", e)
