void init(void) {
	// Set character index to 'H' (0x48)
	*((int*)0xb8000)=0x48;
	// Apply colour index to 'H'
	*((int*)0xb8001)=0xF;
	for(;;) {}
}