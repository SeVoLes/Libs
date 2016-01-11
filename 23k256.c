/************************************************
* Библиотека для работы с 23K256
* SRAM - 256 kBit = 32 768 * 8 bite
* 1024 Pages x 32 byte = 32 768 byts
************************************************/

// Список команд
// Read data from memory array beginning at selected address
#define OPT_READ 0b00000011
// Write data to memory array beginning at selected address
#define OPT_WRITE 0b00000010
// Read STATUS register
#define OPT_RDSR 0b00000101
// Write STATUS register
#define OPT_WRSR 0b00000001

// Чтение статус регистра
//  7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
// --------------------------------
// W/R|W/R| – | – | – | – | – |W/R
// MOD|MOD| 0 | 0 | 0 | 0 | 0 |HOLD
// --------------------------------
// W/R = writable/readable.
// 0 0 = Byte mode (default operation)
// 1 0 = Page mode
// 0 1 = Sequential mode
// 1 1 = Reserved
int8 ext_sram_status_register_read()
{
   int8 data;
   
   SPI_SELECT(SPI_CS_SRAM);

   spi_write(OPT_RDSR);
   data = spi_read(0);
   
   SPI_DESELECT(SPI_CS_SRAM);
   return data;
}

// Запись статус регистра
//  7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
// --------------------------------
// W/R|W/R| – | – | – | – | – |W/R
// MOD|MOD| 0 | 0 | 0 | 0 | 0 |HOLD
// --------------------------------
// W/R = writable/readable.
// 0 0 = Byte mode (default operation)
// 1 0 = Page mode
// 0 1 = Sequential mode
// 1 1 = Reserved
void ext_sram_status_register_write(int8 data)
{
   SPI_SELECT(SPI_CS_SRAM);

   spi_write(OPT_WRSR);
   spi_write(data);

   SPI_DESELECT(SPI_CS_SRAM);
}

// Включает Byte mode - 32 768 байт
void ext_sram_set_byte_mode(void)
{
   ext_sram_status_register_write(0x00);
}

// Включает Page mode - 1024 страниц
void ext_sram_set_page_mode(void)
{
   ext_sram_status_register_write(0x80);
}

// Включает Sequential mode - 32 768 байт
void ext_sram_set_sequential_mode(void)
{
   ext_sram_status_register_write(0xC0);
}

// Чтение 1 БАЙТА
int8 ext_sram_byte_read_sequence(int16 byte_addr)
{
   int8 data;
   
   SPI_SELECT(SPI_CS_SRAM);

   spi_write(OPT_READ);
   spi_write(make8(byte_addr,1));
   spi_write(make8(byte_addr,0));
   data = spi_read(0);

   SPI_DESELECT(SPI_CS_SRAM);
   return data;
}

// Запись 1 БАЙТА
void ext_sram_byte_write_sequence(int16 byte_addr, int8 data)
{
   SPI_SELECT(SPI_CS_SRAM);

   spi_write(OPT_WRITE);
   spi_write(make8(byte_addr,1));
   spi_write(make8(byte_addr,0));
   spi_write(data);

   SPI_DESELECT(SPI_CS_SRAM);
}

// Чтение 1 СТРАНИЦЫ
void ext_sram_page_read_sequence(int16 page_addr, int8 *data)
{
   int i;
   
   SPI_SELECT(SPI_CS_SRAM);

   spi_write(OPT_READ);
   spi_write(make8(page_addr,1));
   spi_write(make8(page_addr,0));
   for (i=0;i<32;i++)
   {
      *data++ = spi_read(0);
   }

   SPI_DESELECT(SPI_CS_SRAM);
}

// Запись 1 СТРАНИЦЫ
void ext_sram_page_write_sequence(int16 page_addr, int8 *data)
{
   int i;
   
   SPI_SELECT(SPI_CS_SRAM);

   spi_write(OPT_WRITE);
   spi_write(make8(page_addr,1));
   spi_write(make8(page_addr,0));
   
   for (i=0;i<32;i++)
   {
      spi_write(*data++);
   }

   SPI_DESELECT(SPI_CS_SRAM);
}

// Чтение X БАЙТ
void ext_sram_sequential_read_sequence(int16 page_addr, int8 *data, int size)
{
   int i;
   
   SPI_SELECT(SPI_CS_SRAM);

   spi_write(OPT_READ);
   spi_write(make8(page_addr,1));
   spi_write(make8(page_addr,0));
   for (i=0;i<size;i++)
   {
      *data++ = spi_read(0);
   }

   SPI_DESELECT(SPI_CS_SRAM);
}

// Запись X БАЙТ
void ext_sram_sequential_write_sequence(int16 page_addr, int8 *data, int size)
{
   int i;
   
   SPI_SELECT(SPI_CS_SRAM);

   spi_write(OPT_WRITE);
   spi_write(make8(page_addr,1));
   spi_write(make8(page_addr,0));
   
   for (i=0;i<size;i++)
   {
      spi_write(*data++);
   }

   SPI_DESELECT(SPI_CS_SRAM);
}
