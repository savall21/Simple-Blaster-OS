# Simple-Blaster-OS
A simple Arduino script for a Star Wars blaster
You can find out more and see other projects at cchobbyfun.com

The script is designed to play blaster and stun sounds. It also run an animation display on a scope display and also an ammo counter that counts down as the fire button is pressed.  The second button serves as a mode change switch modes between blaster and stun.  The code is set for 50 shots. When the ammo counter reaches zero and empty sound is playeed with the fire button is pressed.  The mode change button is pressed when the counter reaches zero and resets the counter back to 50.

The code also lights a 9 segment addressable LED strip red for blaster and blue for stun.  The sounds are played from a DFPLayer mini and loaded on a Micro SD card. The repository includes a set of five sounds. The default sounds are for a E-11 blaster. You can swap the files out for a different blaster.

Commection diagram https://github.com/savall21/Simple-Blaster-OS/blob/47e1a917c7d7898c387c521cf67733bd4df35b0a/simple%20blaster%20OSv2.png

The code its setup to run ona Arduino Nano Every but it will likely run on a wide variety of Arduino boards.  The code displays the ammo counter on a 128x32 OLED screen. The scope animation displays on a 64x32 OLED. Links to purchase the Arduino and screens are below. The screens can probably be substitued with any that are the appropriate size.

Arduino Nano Every
https://www.amazon.com/Arduino-Nano-Every-Single-Board/dp/B07VX7MX27/ref=sr_1_3?crid=2JBRBQ7S2P2PD&keywords=arduino+nano+every&qid=1689296747&sprefix=arduino+nano+every%2Caps%2C94&sr=8-3

64x32 OLED
https://www.ebay.com/itm/165434574694?_trkparms=amclksrc%3DITM%26aid%3D111001%26algo%3DREC.SEED%26ao%3D1%26asc%3D20160811114145%26meid%3D3ab06b348214489c82929b2456f32f78%26pid%3D100667%26rk%3D1%26rkt%3D8%26sd%3D165434574694%26itm%3D165434574694%26pmt%3D1%26noa%3D1%26pg%3D2351460%26brand%3DUnbranded&_trksid=p2351460.c100667.m2042

128x32 OLED
https://www.amazon.com/Teyleten-Robot-Display-SSD1306-Raspberry/dp/B08ZY4YBHL/ref=sr_1_3?crid=1JOKO9DG835S9&keywords=128x32+OLED&qid=1689297324&sprefix=128x32+oled%2Caps%2C87&sr=8-3

DFPlayer Mini
https://www.amazon.com/Anmbest-YX5200-DFPlayer-Supporting-Arduino/dp/B07JGWMPTF/ref=sr_1_5?crid=3AJXSRF0Q3HQ9&keywords=DFPlayer+Mini&qid=1689298204&sprefix=dfplayer+mini%2Caps%2C88&sr=8-5

