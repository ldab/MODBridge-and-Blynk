<h2># MODBridge-and-Blynk</h2>
<h4>This is a working example of open source MODBridge working with the SDM120 Energy meter via MODBUS RTU.</h4>
<p><img style="display: block; margin-left: auto; margin-right: auto;" src="https://cdn.tindiemedia.com/images/resize/65vChiPotfqY2uvdVtShhdQLEnE=/p/full-fit-in/2400x1600/i/348611/products/2018-08-04T20%3A09%3A13.394Z-MODBridge%20side.png" alt="MODBridge MODBUS cloud" width="320" height="315" /></p>
<ul>
<li>MODBridge can be find at:&nbsp;<a href="http://untrol.io/" target="_blank" rel="noopener">untrol.io</a>&nbsp;and&nbsp;<a href="https://www.tindie.com/products/13708/">Tindie</a></li>
<li>SDM120 Energy meter: <a href="http://s.click.aliexpress.com/e/blaegsBQ" target="_parent">Single Phase Din Rail Energy Meter SDM120-Modbus</a></li>
</ul>
<p>The firmware has been writen using PlatformIO which is a nie, neat IDE, check it out:&nbsp;<a href="https://platformio.org/platformio-ide" target="_blank" rel="noopener">Learn how to install PlatformIO IDE</a></p>
<p>If you're feeling brave and want to customize or improve the Firmware Over the Air (OTA), check this link: <a title="Over-the-Air (OTA) update" href="http://docs.platformio.org/en/latest/platforms/espressif8266.html#over-the-air-ota-update" target="_blank" rel="noopener">Over-the-Air (OTA) update</a> basically you change your sketch and upload it by running the following line on PlatformIO terminal:</p>
<blockquote>
<p><span style="color: #ff0000;"><em>platformio run --target upload --upload-port IP_ADDRESS_HERE</em></span></p>
</blockquote>
<h3>OR if you want to use Arduino IDE:&nbsp;<a href="https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html#arduino-ide" target="_blank" rel="noopener">Arduino IDE OTA</a></h3>
<h3>Download Blynk App: <a href="https://www.blynk.cc/getting-started/" target="_blank" rel="noopener"><strong>Getting Started with Blynk</strong></a></h3>
<p>And scan the following QR code to clone the demo App:</p>
<p><a href="https://imgbb.com/"><img src="https://image.ibb.co/icuMPJ/clone_1046666895.png" alt="clone_1046666895" width="220" height="220" border="0" /></a> <a href="https://ibb.co/iXgPdd"><img src="https://preview.ibb.co/i1pXWy/Screenshot_20180720_101705_Blynk.jpg" alt="Screenshot_20180720_101705_Blynk" width="250" height="444" border="0" /></a> <a href="https://ibb.co/dPpAJd"><img src="https://preview.ibb.co/fwQjdd/Screenshot_20180720_101710_Blynk.jpg" alt="Screenshot_20180720_101710_Blynk" width="250" height="444" border="0" /></a></p>
<p>To do: -Make registers and conf user friendly</p>
