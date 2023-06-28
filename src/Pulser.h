#ifndef __PULSER__
#define __PULSER__

namespace Pulser {

    /*
     * Class Pulse represents the encoding for a single data pulse.
     * This is typically a timed transition from low to high state and back,
     * with defined durations in the high and low states.
     * Typically pulses encode 0s and 1s and are used in sequences,
     * but they can also be used standalone to represent sync pulses,
     * message terminators, or other non-standard signals.
     */
    class Pulse {
        public:
            Pulse();
            Pulse(int startState, int startLen, int endState, int endLen);
            int startState() const { return _startState; }
            int startLen()   const { return _startLen; }
            int endState()   const { return _endState; }
            int endLen()     const { return _endLen; }

        private:
            int _startState;
            int _startLen;
            int _endState;
            int _endLen;
    };

    /*
     * Class Sequence represents a series of pulses, all using the same data pulse encoding.
     */
    class Sequence {
        public:
            Sequence();
	    Sequence(const Pulse &zero, const Pulse &one, const char *bitstring, int length);
	    ~Sequence();
            void setZero(const Pulse &p) { _zero = p; }
            void setOne(const Pulse &p) { _one = p; }
            const Pulse& zero() const { return _zero; }
            const Pulse& one() const { return _one; }
            void setBits(const char *bitstring, int length);
            const char *bits() const;

        private:
            Pulse _zero, _one;
            char *_seq;
            int _len;
    };

    /*
     * Class Pulser sends out individual pulses and pulse sequences to an output pin.
     */
    class Pulser {
        public:
            Pulser(int pin);
            void setPin(int pin);
            void sendPulse(const Pulse &p) const;
            void sendSequence(const Sequence &s) const;

        private:
            int _pin;
    };
}

#endif //__PULSER__
