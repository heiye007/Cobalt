void init(void) {
	// Set character index to 'H' (0x48)
	*((int*)0xb8000)=0x48;
	// Apply colour index to 'H'
	*((int*)0xb8001)=0xF;
	// Set character index to 'e' (0x65)
	*((int*)0xb8002)=0x65;
	// Apply colour index to 'e'
	*((int*)0xb8003)=0xF;
	// Set character index to 'l' (0x6c)
	*((int*)0xb8004)=0x6c;
	// Apply colour index to 'l'
	*((int*)0xb8005)=0xF;
	// Set character index to 'l' (0x6c)
	*((int*)0xb8006)=0x6c;
	// Apply colour index to 'l'
	*((int*)0xb8007)=0xF;
	// Set character index to 'o' (0x6f)
	*((int*)0xb8008)=0x6f;
	// Apply colour index to 'o'
	*((int*)0xb8009)=0xF;
	for(;;) {}
}