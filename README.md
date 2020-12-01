<pre>

/* float24, Copyright (c) 2020 Yuriy Yakimenko */ 

####  Floating point number using 24 bits.
=========================================

I needed  to store a floating point value  with maximum possible  precision, but
had  only  three bytes (as opposed to regular four) for it in memory. I am aware
of 16-bit floats (aka mini-floats or half-floats) but I wanted to take advantage
of using all three bytes for best accuracy. 

In my  case I needed to store non-negative numbers between 0 and  65536  (2^16),
which made a decision  of how many bits to  allocate  for mantissa and  exponent
very simple. Since I did not  need negative  values, I also did  not need a sign
bit. 

With maximum value of 2^16 I needed to allocate 5 bits for exponent (effectively
storing values from -15 (corresponds to 1) to 15 (corresponds to 31)). Note that
when   exponent   bits    contain    zero,   we    store   denormalized   number
(https://en.wikipedia.org/wiki/Denormal_number). 

I   also  made   this  implementation  compatible   with  both  big-endian   and
little-endian machines. 

#### Known issues: 
================= 

In case  you would  like to modify  this code to store  a signed 24-bit floating
point value, you will need to add a sign bit and correspondingly remove this bit
from either mantissa or exponsent (depending on your required range/accuracy). 

</pre> 

#### License 

[MIT](https://choosealicense.com/licenses/mit/) 
