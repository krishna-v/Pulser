#include <Arduino.h>
#include "pulser.h"

namespace Pulser {
    Pulse::Pulse() {
        _startState = HIGH;
        _startLen = 0;
        _endState = LOW;
        _endLen = 0;
    }
    
    Pulse::Pulse(int startState, int startLen, int endState, int endLen) {
        _startState = startState;
        _startLen   = startLen;
        _endState   = endState;
        _endLen     = endLen;
    }
    
    Sequence::Sequence() {
        _len = 0;
    }
    
    Sequence::Sequence(const Pulse &zero, const Pulse &one,
    			    const char *bitstring, int length) {
        setZero(zero);
        setOne(one);
        _len = 0;
        setBits(bitstring, length);
    }
    
    void Sequence::setBits(const char *bitstring, int length) {
        if(length <= 0) return;
        if(_len > 0) free(_seq);
        _seq = (char *)malloc(length);
        memcpy(_seq, bitstring, length+1);
        _len = length;
        _seq[_len] = '\0';
    }
    
    const char *Sequence::bits() const {
        if(_len == 0) return "";
        return _seq;
    }
    
    Sequence::~Sequence() {
        if(_len > 0) free(_seq);
    }
    
    Pulser::Pulser(int pin) {
        setPin(pin);
    }
    
    void Pulser::setPin(int pin) {
        _pin = pin;
        pinMode(_pin, OUTPUT);
    }
    
    void Pulser::sendPulse(const Pulse &p) const {
        digitalWrite(_pin, p.startState());
        delayMicroseconds(p.startLen());
        digitalWrite(_pin, p.endState());
        delayMicroseconds(p.endLen());
    }
    
    void Pulser::sendSequence(const Sequence &seq) const {
        const char *bits = seq.bits();
        for(int i = 0; bits[i] != '\0'; i++) {
    	if(bits[i] == '0') sendPulse(seq.zero());
    	else if(bits[i] == '1') sendPulse(seq.one());
        }
    }
}
