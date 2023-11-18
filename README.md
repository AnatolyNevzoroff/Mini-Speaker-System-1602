# Mini-Speaker-System-1602
Mini Desktop Speaker System for Personal Computer

Smartphones and personal computers have become sources of media content for the majority. 
But to bring the audio component to your ears, you also need headphones or acoustically 
designed speakers with an amplifier. Everything is clear with headphones - this is an 
irreplaceable thing if you want to listen to music loud enough, but without disturbing others, 
with good quality and for relatively little money. But headphones are not suitable for all cases. 
Industrial solutions for desktop near-field acoustics are usually divided into two types:
1. Inexpensive, compact, as long as they play, without claims to quality.
2. Large, two-way, already with acceptable sound quality (sometimes even with 3 speakers) capable
of sounding a small dance floor, apartment and even neighbors; when you try to turn up the
volume (after receiving a scolding from someone close to you and ruining relations with neighbors),
acoustics turn into a solution of the first type, but much more expensive and larger.
In general, there are solutions that are quite suitable in terms of functionality and quality,
but the price of the issue does not suit me personally.
The conclusion from everything written above is that if you want to do it well, do it yourself.

We take 3 or 4 inch wideband speakers...
Modern broadband speakers are not at all the same as those used in old radios; there is no bi-amping 
or filters after the amplifier, simple calculation and manufacturing.
The speakers must be of the highest quality, for example mine; in the passport, in addition to the 
acoustic characteristics, the recommended volume of the box with the bass reflex and even the 
dimensions of the recommended bass reflex were indicated. In my case, the volume turned out to be 
about 3 liters, the length of the bass reflex pipe was 12 cm. The diameter was 2.4 cm.
In most cases, for 3" speakers the volume is 3 liters, for 4" - 5 liters. In order not to get confused, 
count the volume in decimeters, to save the space occupied on the table, I raised the wooden boxes up, 
at the same time raising the speakers to a height close to the height of the head (ears) above the table.
Yes, you will have to do some carpentry, make noise and litter... under the pleasant smell of pine and forest. 
You will have to tinker with the holes, fit the block of buttons and encoder tightly, without special machines, 
painting booths, with an ordinary brush.
In one of the boxes we place the BD37534FV audio processor controlled by Arduino and two TDA7293 microcircuits, 
providing the rear wall with a cooling radiator. Connect and you're done! There are of course nuances... :-))

I didn’t immediately realize that the TDA7293 actually has the ability to be controlled from a microcontroller. 
If the state at pin 9 (standby) is LOW (less than 2.5 V), the consumption of the microcircuit is a few milliamps! 
And she doesn’t worry about any problems with nutritional imbalance. But after waiting a pause of about 1 second (in my case),
we supply +5 volts from the controller (through a 22 kOhm current-quenching resistor) and the microcircuit turns 
on absolutely silently. The situation is similar with pin 10 (MUTE). For a smoother switch-on, we bypass the input 
with electrolyte and get a short-term, but quite pleasant increase in the signal level in the HIGH state.



Housing: homemade from furniture pine board, does not resonate at relatively low system powers, easy to process, inexpensive.

Weight 3.8 kg. (both speakers including power cord)
Overall dimensions (H x W x D): 295 x 145 x 145 (without legs and protruding body elements)
Output power 2 x 18 W (at 8 Ohm load)
Total harmonic distortion (for headphones) ... 0.003% (in the signal range 10Hz-20KHz)
Total harmonic distortion (output to Subwoofers) ... 0.002% (in the range 400Hz-30KHz)
Total harmonic distortion (output into 8 Ohm load) ... 0.05% (in the range 20Hz-20KHz)
Crosstalk between channels... -100 dB
Crosstalk between input selectors... -100 dB
Input Preamp 0 to 20 dB
MUTE mode... -105 dB
Volume adjustment -79 to +15 dB
Tone adjustment HF, MF, LF... ± 20 dB
Output attenuator -79 to 15 dB
Loudness compensation from 0 to 20 dB
Shift of the center band of the tone controls 4 options for 3 bands
Changing the quality factor of the tone controls 2 or 4 options depending on the frequency band
LPF phase shift
Control 4 buttons on the front panel + encoder button + 9 buttons on the IR remote control
Ask questions in the comments or by personal email tolikn@yandex.ru
