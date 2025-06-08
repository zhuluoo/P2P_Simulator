# P2P Video Streaming Simulator

This project simulates a peer-to-peer (P2P) video streaming network, where one server and multiple client nodes collaborate to distribute video data efficiently. It's designed as a Data Structures course project, and demonstrates core ideas like:

- Randomized graph construction
- Simulated data transmission and caching
- Node failure and recovery
- Visualization with Qt (zoom, pan, delay tracking)


# Features Implemented

- Server generates video data at 30KB/s, divided into 1KB blocks
- Clients fetch and cache data from neighbors
- Playback simulated and per-block delay recorded
- Supports node exit and recovery with dynamic neighbor reconnection
- Interactive Qt GUI:
  - Zoom and pan the network
  - Visualize transmission events
  - Inspect delay per client
  - Remove clients and rerun simulation


# How to Run

1. Install [Qt 6] 
2. Clone this repo and open in Qt Creator, or run:

# bash
qmake viz.pro
make
./viz
