BIN := life

$(BIN):
	spasm $(BIN).asm $(BIN).8xp

clean:
	rm -f $(BIN).8xp

.PHONY: clean
