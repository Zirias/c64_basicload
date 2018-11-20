.code
		.word	$0334		; load address (datasette buffer)

		ldx	#$1		; index for writing start address
		stx	$2		; init marker for hi-/lo- nibble
lineloop:	jsr	$0073		; get next character from BASIC
		beq	eol		; 0 -> end of BASIC line
hexloop:	jsr	$0073		; next char
		beq	done		; 0 -> end of BASIC program
		cmp	#'"'
		beq	checkend	; another quote, check whether done
		sbc	#$30		; subtract offset for '0'
		cmp	#$11
		bcc	digit		; if below 10, it was a decimal digit
		sbc	#$7		; otherwise subtract diff '10' to 'A'
digit:		lsr	$2		; check bit #0 of marker
		bcc	seconddigit	; not set -> second digit (lo nibble)
		asl			; otherwise shift 4 bits
		asl
		asl
		asl
		sta	$fb		; store to temporary
		bcc	hexloop		; and continue at next digit
seconddigit:	ora	$fb		; combine with hi nibble
		sec
		rol	$2		; shift a 1 back into bit #0
		cpx	#$3		; start address already read?
		bne	setpointers	; if not, set pointers
write:		sta	$0101		; store to dest (placeholder)
		inc	write+1		; next address
		bne	hexloop		; and repeat
		inc	write+2		; on overflow, also increment hi byte
		bne	hexloop
setpointers:	sta	done,x		; write pointer to start program
		sta	write,x		; write pointer to store program
		inx			; next index
		bne	hexloop		; and repeat
checkend:	jsr	$0073		; on '"', get next character
		beq	eol		; 0 -> end of BASIC line
done:		jmp	$af08		; start our program (placeholder)

eol:		ldy	#$5		; load first character of next
		lda	($7a),y		; BASIC line
		cmp	#'"'
		bne	done		; if it's not '"', we're done loading
		ldy	#$2		; check next BASIC line pointer
		lda	($7a),y
		beq	done		; 0 -> end of BASIC program, so done
		iny
		lda	($7a),y		; copy next BASIC line number
		sta	$39		; to ZP location where BASIC expects it
		iny
		lda	($7a),y
		sta	$3a
		clc
		tya
		adc	$7a		; move CHRGET pointer to start of
		sta	$7a		; next BASIC line
		bcc	lineloop	; and continue reading
		inc	$7b		; adjust hi-byte on carry
		bne	lineloop
