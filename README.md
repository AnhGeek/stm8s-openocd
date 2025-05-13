To build:
make all

To debug:
* Start gdb server:
sudo openocd -f interface/stlink-dap.cfg -f target/stm8s003.cfg -c "init" -c "reset halt"

* Stard gdb session:
stm8-gdb _out/output.elf --tui
<return>
<return>
start

Note: after type "start" then enter, you will see symbols are loaded.


To flash, go to openocd's script folder
 cd /usr/local/share/openocd/scripts
then run:
 sudo openocd -f interface/stlink-dap.cfg -f target/stm8s003.cfg -c "init" -c "program_device /home/hoanganh/stm8_gdb/stm8s-openocd/_out/output.hex 0"