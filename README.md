# DRAGON

IMPORTANT NOTE: This repo is currently undergoing a major overhaul (see develop branch for updates), and this is the master branch for the old structure. Once v0.1 of the new module is ready, this branch will be renamed (but preserved for posterity) and a new master will take its place.

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

- This is a major bottleneck at high sample rates. We need to think of a more
    efficient way to do this, or scrap the PDU idea entirely.
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
1. GRCLBase: Base class thati extends OpenCL functionality to GR blocks.
Modifed version of class found [here](https://github.com/ghostop14/gr-clenabled).
## Feature Wishlist
1. Real-time range-doppler map generation (something like gr-fosphor or the
spectrogram plot in the qt gui sink block)
2. Extended waveform/phase code generation capabilities (real-time optimization?)
3. Make doxygen index.html available directly from github
4. RFNoC integration

