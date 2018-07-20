<h2># MODBridge-and-Blynk</h2>
<h4>This is a working example of open source MODBridge working with the SDM120 Energy meter.</h4>
<ul>
<li>MODBridge can be find at:&nbsp;<a href="http://untrol.io/" target="_blank" rel="noopener">untrol.io</a>&nbsp;and&nbsp;<a href="https://www.tindie.com/" target="_blank" rel="noopener">Tindie</a></li>
<li>SDM120 Energy meter: <a href="https://www.aliexpress.com/item/SDM120-Modbus-1-phase-2-wire-0-25-5-45-A-230V-RS485-Modbus-kwh-kvarh/32515923674.html?spm=2114.search0104.3.1.78072ab6FFreXY&amp;ws_ab_test=searchweb0_0,searchweb201602_5_10152_10065_10151_10344_10068_10342_10547_10343_10340_10059_10341_10548_10696_100031_10084_10083_10103_10618_10307_10624_10623_10622_10621_10620,searchweb201603_13,ppcSwitch_5&amp;algo_expid=3706244a-829d-4feb-9234-e7d02d62fb22-0&amp;algo_pvid=3706244a-829d-4feb-9234-e7d02d62fb22&amp;transAbTest=ae803_1&amp;priceBeautifyAB=0" target="_blank" rel="noopener">SDM120 Modbus, Single-Phase</a></li>
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
