# Connectwith arduino and esp32-cam

setting : 
          arduino           esp32-cam
          5V       ->       5V
          gnd      ->       gnd
          RX       ->       U0R
          TX       ->       U0T

esp32-cam board : esp32 wrover module
          speed : 115200
          frequency : 40HZ
          Partition Scheme : Huge APP(3MB NO OTA/1MB SPIEFS)

arduino : reset -> gnd
esp32-cam upload code : IO0 -> gnd

servo -> esp32-cam's GPIO12
         5V -> 5V
         gnd -> gnd

step 1 : upload esp code to esp32-cam 
step 2 : pull out IO0->gnd wire 
step 3 : run esp code 
step 4 : pull out reset->gnd wire and RX/TX wire
step 5 : upload uno to arduino board
step 6 : RX -> U0T, TX -> U0R 