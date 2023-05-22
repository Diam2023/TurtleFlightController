## Data Structure




## PIN Defination
* LSM6DSOWTR -> I2C1
 - INT1 -> PB2
* BMP280 -> SPI1
 - CS -> PA0
* W25Q16 -> SPI1
 - CS -> PA4


## OpenOCD Download
* "/usr/bin/openocd" -f ./openocd.cfg -c "program build/Flight.elf verify reset exit"
