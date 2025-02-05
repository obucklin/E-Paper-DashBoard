// custom_epd_w21.cpp
#include "GDEP073E01.h"
#include "Display_EPD_W21.h"
#include "Display_EPD_W21_spi.h"

void GDEP073E01::setup() {
  // Initialize SPI
  spi_ = new SPIClass(HSPI);
  spi_->begin(SCK, MISO, MOSI, SS);

  // Initialize pins
  reset_pin_->setup();
  dc_pin_->setup();
  cs_pin_->setup();
  busy_pin_->setup();

  // Hardware reset
  reset_pin_->digital_write(true);
  delay(10);
  reset_pin_->digital_write(false);
  delay(10);
  reset_pin_->digital_write(true);
  delay(10);

  initialize_display();
}

void GDEP073E01::initialize_display() {

  EPD_W21_RST_0;    // Module reset
  delay(10);//At least 10ms delay 
  EPD_W21_RST_1;
  delay(10);//At least 10ms delay 

  
  send_command(0xAA);    // CMDH
  send_data(0x49);
  send_data(0x55);
  send_data(0x20);
  send_data(0x08);
  send_data(0x09);
  send_data(0x18);

  send_command(PWRR);//
  send_data(0x3F);
  
  send_command(PSR);  
  send_data(0x5F);
  send_data(0x69);
  
  send_command(POFS);
  send_data(0x00);
  send_data(0x54);
  send_data(0x00);
  send_data(0x44); 

  send_command(BTST1);
  send_data(0x40);
  send_data(0x1F);
  send_data(0x1F);
  send_data(0x2C);
  
  send_command(BTST2);
  send_data(0x6F);
  send_data(0x1F);
  send_data(0x17);
  send_data(0x49);

  send_command(BTST3);
  send_data(0x6F);
  send_data(0x1F);
  send_data(0x1F);
  send_data(0x22);
  send_command(PLL);
  send_data(0x08);
  send_command(CDI);
  send_data(0x3F);
  
  send_command(TCON);
  send_data(0x02);
  send_data(0x00);
  
  send_command(TRES);
  send_data(0x03);
  send_data(0x20);
  send_data(0x01); 
  send_data(0xE0);

  send_command(T_VDCS);
  send_data(0x01);
  
  send_command(PWS);
  send_data(0x2F);

  send_command(0x04);     //PWR on  
  wait_until_idle();          //waiting for the electronic paper IC to release the idle signal

}

void GDEP073E01::wait_until_idle() {
  while(busy_pin_->digital_read() == LOW) {
    delay(1);
  }
}

void GDEP073E01::send_command(uint8_t command) {
  dc_pin_->digital_write(LOW);
  cs_pin_->digital_write(LOW);
  spi_->transfer(command);
  cs_pin_->digital_write(HIGH);
}

void GDEP073E01::send_data(uint8_t data) {
  dc_pin_->digital_write(HIGH);
  cs_pin_->digital_write(LOW);
  spi_->transfer(data);
  cs_pin_->digital_write(HIGH);
}

void GDEP073E01::display_white() {
  unsigned long i;
  
  send_command(DTM);
  {
    for(i=0;i<192000;i++)
    {
      send_data(White);
    }
  }
  send_command(PON);
  wait_until_idle();
  
  //20211212
  //Second setting
  send_command(BTST2);
  send_data(0x6F);
  send_data(0x1F);
  send_data(0x17);
  send_data(0x49);
    
  
  send_command(DRF);
  send_data(0x00);
  wait_until_idle();
  
 
  send_command(POF);
  send_data(0x00);
  wait_until_idle();
}
// Implement other required methods...