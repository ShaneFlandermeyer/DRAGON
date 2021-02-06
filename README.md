# DRAGON
A GNU Radio-based signal processing library for the University of Oklahoma's
DRAGON research group. The ultimate goal for this project is to create a
streamlined framework for performing radar functions on GR-compatible
software-defined radios. This framework currently consists of a GR module and
associated (currently deprecated) matlab implementations of blocks in the
module.

The following blocks are available for use in GNU Radio:

## Math Blocks
1. Complex Exponential
2. FFT Convolution
3. Time-Domain Convolution
4. Linear to dB (CPU and GPU)
5. Differentiation
6. Integration

## PDU Blocks
1. PDU to Stream
(Modified version of block from [here](https://github.com/sandialabs/gr-pdu_utils))

## Radar Blocks
1. Moving Target Simulator

## Vector Blocks
1. Oversample Vector
2. Pad Vector

## Waveform Blocks
1. Pulse Shaper (Applies CPM shaping filters to input)
2. Phase Code Generator (Currently only P4 codes, but easily extensible)
3. Linear Frequency-Modulated (LFM) Chirp Generator
4. Polyphase-Coded Frequency-Modulated (PCFM) Modulator

## Other Functions
1. GRCLBase: Base class that extends OpenCL functionality to GR blocks.
Modifed version of class found [here](ht.tps://github.com/ghostop14/gr-clenabled).
## Feature Wishlist
1. RFNoC integration
2. OpenCL/GL range-doppler map generation (similar to gr-fosphor)
3. Extended waveform/phase code generation capabilities
4. Make Doxygen Documentation available in github

