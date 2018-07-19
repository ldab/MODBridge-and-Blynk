<p># MODBridge-and-Blynk</p>
<h4>This is a working example of open source MODBridge working with the SDM120 Energy meter.</h4>
<ul>
<li>MODBridge can be find at:&nbsp;<a href="https://www.untrol.io" target="_blank" rel="noopener">untrol.io</a>&nbsp;and&nbsp;<a href="https://www.tindie.com/" target="_blank" rel="noopener">Tindie</a></li>
<li>SDM120 Energy meter: <a href="http://gestyy.com/wJ2uKW" target="_blank" rel="noopener">SDM120 Modbus, Single-Phase</a></li>
</ul>
<p>The firmware has been writen using PlatformIO which is a nice, neat IDE, check it out:&nbsp;<a href="https://platformio.org/get-started" target="_blank" rel="noopener">Learn how to install PlatformIO IDE</a></p>
<p>If you're feeling brave and want to customize or improve the Firmware Over the Air (OTA), check this link: <a title="Over-the-Air (OTA) update" href="http://docs.platformio.org/en/latest/platforms/espressif8266.html#over-the-air-ota-update" target="_blank" rel="noopener">Over-the-Air (OTA) update</a> basically you change your sketch and upload it by running the following line on PlatformIO terminal:</p>
<blockquote>
<p><span style="color: #ff0000;"><em>platformio run --target upload --upload-port IP_ADDRESS_HERE</em></span></p>
</blockquote>
<h3>Download Blynk App: <a href="https://www.blynk.cc/getting-started/" target="_blank" rel="noopener"><strong>Getting Started with Blynk</strong></a></h3>
<p>And scan the following QR code to clone the demo App:</p>
<p><a href="https://imgbb.com/"><img src="https://image.ibb.co/icuMPJ/clone_1046666895.png" alt="clone_1046666895" width="220" height="220" border="0" /></a> <a href="https://ibb.co/iXgPdd"><img src="https://preview.ibb.co/i1pXWy/Screenshot_20180720_101705_Blynk.jpg" alt="Screenshot_20180720_101705_Blynk" width="250" height="444" border="0" /></a> <a href="https://ibb.co/dPpAJd"><img src="https://preview.ibb.co/fwQjdd/Screenshot_20180720_101710_Blynk.jpg" alt="Screenshot_20180720_101710_Blynk" width="250" height="444" border="0" /></a></p>
<p>To do: -Make registers and conf user friendly</p>
