/*
使用STM32F103开发板，实现DMA完成 M(SRAM)->P(USART1) 的数据传输，在传输数据的过程中，让CPU去执行LED的闪烁。
DMA 传输数据时，不会占用CPU，CPU可以干自己的事情，两者互不影响。
*/