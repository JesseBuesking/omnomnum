#include "scanner.def.h"
#include "parser.h"
#include "scan.h"
#include "readmem.h"

#define TOKEN_SEPARATOR 10000

int omnomnum_scanner_start(scanstate *ss) {
    scanner_enter(ss);
//std:
    /*!re2c
        SEPARATOR       = [ \r\n\t\f\-]+;
        END             = "\x00";
        //CHARS         = [^ \r\n\t\f\-\x00]+; // not a separator
        CHARS           = [^]; // not a separator
        // see http://re2c.org/examples/example_07.html for ideas of other valid
        // types of numbers
        NUMBER          = ([0-9]* "." [0-9]+) | ([0-9]+);
        THREE_PART_DATE = [0-9]{2,4} "/" [0-9]{2,4} "/" [0-9]{2,4};
        TWO_PART_DATE   = [0-9]+ "/" [0-9]+;

        THREE_PART_DATE { return TOKEN_CHARACTERS; }
        TWO_PART_DATE { return TOKEN_CHARACTERS; }
        NUMBER { return TOKEN_NUMBER; }

        'and' { return TOKEN_AND; }
        'and a' { return TOKEN_AND_A; }

        'negative' { return TOKEN_NEGATIVE; }

        'zero' { return TOKEN_ZERO; }
        'one' { return TOKEN_ONE; }
        'two' { return TOKEN_TWO; }
        'three' { return TOKEN_THREE; }
        'four' { return TOKEN_FOUR; }
        'five' { return TOKEN_FIVE; }
        'six' { return TOKEN_SIX; }
        'seven' { return TOKEN_SEVEN; }
        'eight' { return TOKEN_EIGHT; }
        'nine' { return TOKEN_NINE; }
        'ten' { return TOKEN_TEN; }
        'eleven' { return TOKEN_ELEVEN; }
        'twelve' { return TOKEN_TWELVE; }
        'thirteen' { return TOKEN_THIRTEEN; }
        'fourteen' { return TOKEN_FOURTEEN; }
        'fifteen' { return TOKEN_FIFTEEN; }
        'sixteen' { return TOKEN_SIXTEEN; }
        'seventeen' { return TOKEN_SEVENTEEN; }
        'eighteen' { return TOKEN_EIGHTEEN; }
        'nineteen' { return TOKEN_NINETEEN; }
        'ninteen' { return TOKEN_NINETEEN; }

        'twenty' { return TOKEN_TWENTY; }
        'thirty' { return TOKEN_THIRTY; }
        'forty' { return TOKEN_FORTY; }
        'fourty' { return TOKEN_FORTY; }
        'fifty' { return TOKEN_FIFTY; }
        'sixty' { return TOKEN_SIXTY; }
        'seventy' { return TOKEN_SEVENTY; }
        'eighty' { return TOKEN_EIGHTY; }
        'ninety' { return TOKEN_NINETY; }

        'a hundred' { return TOKEN_HUNDRED; }
        'hundred' { return TOKEN_HUNDRED; }
        'a thousand' { return TOKEN_THOUSAND; }
        'thousand' { return TOKEN_THOUSAND; }
        'a million' { return TOKEN_MILLION; }
        'million' { return TOKEN_MILLION; }
        'a billion' { return TOKEN_BILLION; }
        'billion' { return TOKEN_BILLION; }
        'a trillion' { return TOKEN_TRILLION; }
        'trillion' { return TOKEN_TRILLION; }

        'first' { return TOKEN_FIRST; }
        'second' { return TOKEN_SECOND; }
        'third' { return TOKEN_THIRD; }
        'fourth' { return TOKEN_FOURTH; }
        'fifth' { return TOKEN_FIFTH; }
        'sixth' { return TOKEN_SIXTH; }
        'seventh' { return TOKEN_SEVENTH; }
        'eighth' { return TOKEN_EIGHTH; }
        'nineth' { return TOKEN_NINTH; }
        'ninth' { return TOKEN_NINTH; }

        'tenth' { return TOKEN_TENTH; }
        'eleventh' { return TOKEN_ELEVENTH; }
        'twelfth' { return TOKEN_TWELFTH; }
        'thirteenth' { return TOKEN_THIRTEENTH; }
        'fourteenth' { return TOKEN_FOURTEENTH; }
        'fifteenth' { return TOKEN_FIFTEENTH; }
        'sixteenth' { return TOKEN_SIXTEENTH; }
        'seventeenth' { return TOKEN_SEVENTEENTH; }
        'eighteenth' { return TOKEN_EIGHTEENTH; }
        'nineteenth' { return TOKEN_NINETEENTH; }

        'twentieth' { return TOKEN_TWENTIETH; }
        'thirtieth' { return TOKEN_THIRTIETH; }
        'fourtieth' { return TOKEN_FOURTIETH; }
        'fiftieth' { return TOKEN_FIFTIETH; }
        'sixtieth' { return TOKEN_SIXTIETH; }
        'seventieth' { return TOKEN_SEVENTIETH; }
        'eightieth' { return TOKEN_EIGHTIETH; }
        'ninetieth' { return TOKEN_NINETIETH; }

        'hundredth' { return TOKEN_HUNDREDTH; }
        'thousandth' { return TOKEN_THOUSANDTH; }
        'millionth' { return TOKEN_MILLIONTH; }
        'billionth' { return TOKEN_BILLIONTH; }
        'trillionth' { return TOKEN_TRILLIONTH; }

        'quarter' { return TOKEN_QUARTER; }
        'half' { return TOKEN_HALF; }

        //'firsts' { return TOKEN_FIRSTS; }
        //'seconds' { return TOKEN_SECONDS; }
        //'thirds' { return TOKEN_THIRDS; }
        //'fourths' { return TOKEN_FOURTHS; }
        //'fifths' { return TOKEN_FIFTHS; }
        //'sixths' { return TOKEN_SIXTHS; }
        //'sevenths' { return TOKEN_SEVENTHS; }
        //'eighths' { return TOKEN_EIGHTHS; }
        //'nineths' { return TOKEN_NINTHS; }
        //'ninths' { return TOKEN_NINTHS; }

        //'tenths' { return TOKEN_TENTHS; }
        //'elevenths' { return TOKEN_ELEVENTHS; }
        //'twelfths' { return TOKEN_TWELFTHS; }
        //'thirteenths' { return TOKEN_THIRTEENTHS; }
        //'fourteenths' { return TOKEN_FOURTEENTHS; }
        //'fifteenths' { return TOKEN_FIFTEENTHS; }
        //'sixteenths' { return TOKEN_SIXTEENTHS; }
        //'seventeenths' { return TOKEN_SEVENTEENTHS; }
        //'eighteenths' { return TOKEN_EIGHTEENTHS; }
        //'nineteenths' { return TOKEN_NINETEENTHS; }

        //'twentieths' { return TOKEN_TWENTIETHS; }
        //'thirtieths' { return TOKEN_THIRTIETHS; }
        //'fourtieths' { return TOKEN_FOURTIETHS; }
        //'fiftieths' { return TOKEN_FIFTIETHS; }
        //'sixtieths' { return TOKEN_SIXTIETHS; }
        //'seventieths' { return TOKEN_SEVENTIETHS; }
        //'eightieths' { return TOKEN_EIGHTIETHS; }
        //'ninetieths' { return TOKEN_NINETIETHS; }

        //'hundredths' { return TOKEN_HUNDREDTHS; }
        //'thousandths' { return TOKEN_THOUSANDTHS; }
        //'millionths' { return TOKEN_MILLIONTHS; }
        //'billionths' { return TOKEN_BILLIONTHS; }
        //'trillionths' { return TOKEN_TRILLIONTHS; }

        //'quarters' { return TOKEN_QUARTERS; }
        //'halves' { return TOKEN_HALVES; }

        //SEPARATOR { goto std; }
        SEPARATOR { return TOKEN_SEPARATOR; }

        END { return 0; }

        CHARS { return TOKEN_CHARACTERS; }
    */
}
