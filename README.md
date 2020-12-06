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

#### Update to support signed 24-bit floats:
===========================================

To represent  signed 24-bit  number,  use float24_s structure. It  uses the same
number of  bits  for exponent but one  bit for sign, and correspondingly 18, not
19 bits for mantissa. 

#### Known issues: 
================= 

In case  you  would  like  to modify this code to store  a  different range, you 
will need  to change the number of exponent bits accordingly using FP24_EXP_BITS 
or FP24S_EXP_BITS. Everything else is done automatically. 

</pre> 

#### License 

[MIT](https://choosealicense.com/licenses/mit/) 
