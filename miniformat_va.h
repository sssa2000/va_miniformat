//////////////////////////////////// JJ ///////////////////////////////////
// -------------------------------------------------------------------------
//  File Name        : miniformat.h
//  Author           : Jaewon Jung
//  Description      : A very simple type-safe string formatting library
//    - A header-only library
//    - Minimal dependency
//    - Take an output parameter as an instance of a 'string' class
//     -- Not necessarily std::string
//     -- Any string class with following interfaces will do:
//      --- Get a length of the string
//      --- Append a character or a string to it
//      --- Reserve a specific amount of memory for it
//      --- Get a non-const char pointer to a specific location in the string  
//     -- MFC CString adapter is also provided by default
//    - Faster than sprintf
//     -- Try to minimize the memory allocation
//    - Position-based format specifier
//    - Try to be as similar as possible to familiar printf format specifiers
//    - Support only a (arguably most useful) subset among a plethora of printf format specifiers 
//     -- Integer(signed/unsigned,32bit/64bit), floating-point number, c-string are supported as an argument
//     -- Width, precision options supported
//  References        : 
//    - http://code.google.com/p/stringencoders/wiki/NumToA
//    - http://altdevblogaday.com/2011/07/12/abusing-the-c-preprocessor/
//    - https://github.com/c42f/tinyformat
//    -  https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920
// -------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////

#pragma  once

#include <cstdio>
#include <cassert>
#include <cstdint>
#include <cstring>

#define _FORMAT_JOIN(x,y)		_FORMAT_JOIN2(x,y)
#define _FORMAT_JOIN2(x,y)	x##y

#define _FORMAT_LIST_0(x)
#define _FORMAT_LIST_1(x)		x##1
#define _FORMAT_LIST_2(x)		_FORMAT_LIST_1(x), x##2
#define _FORMAT_LIST_3(x)		_FORMAT_LIST_2(x), x##3
#define _FORMAT_LIST_4(x)		_FORMAT_LIST_3(x), x##4
#define _FORMAT_LIST_5(x)		_FORMAT_LIST_4(x), x##5
#define _FORMAT_LIST_6(x)		_FORMAT_LIST_5(x), x##6
#define _FORMAT_LIST_7(x)		_FORMAT_LIST_6(x), x##7
#define _FORMAT_LIST_8(x)		_FORMAT_LIST_7(x), x##8
#define _FORMAT_LIST_9(x)		_FORMAT_LIST_8(x), x##9

#define _FORMAT_LIST(cnt,x)		_FORMAT_JOIN(_FORMAT_LIST_,cnt)(x)

#define _FORMAT_ARG_0(P,p)
#define _FORMAT_ARG_1(P,p)		P##1 p##1
#define _FORMAT_ARG_2(P,p)		_FORMAT_ARG_1(P,p), P##2 p##2
#define _FORMAT_ARG_3(P,p)		_FORMAT_ARG_2(P,p), P##3 p##3
#define _FORMAT_ARG_4(P,p)		_FORMAT_ARG_3(P,p), P##4 p##4
#define _FORMAT_ARG_5(P,p)		_FORMAT_ARG_4(P,p), P##5 p##5
#define _FORMAT_ARG_6(P,p)		_FORMAT_ARG_5(P,p), P##6 p##6
#define _FORMAT_ARG_7(P,p)		_FORMAT_ARG_6(P,p), P##7 p##7
#define _FORMAT_ARG_8(P,p)		_FORMAT_ARG_7(P,p), P##8 p##8
#define _FORMAT_ARG_9(P,p)		_FORMAT_ARG_8(P,p), P##9 p##9

#define _FORMAT_ARG(cnt,P,p)		_FORMAT_JOIN(_FORMAT_ARG_,cnt)(P,p)

namespace miniformat {}
namespace mini = miniformat;

namespace miniformat
{
	template<typename String>
	inline void format(String& outputText);

	template<typename String,typename... TS>
	void format(String& outputText, const char *formatText,TS... args);

	template<typename... TS>
	void format_va(TS... args);

	template<typename String,typename T,typename... TS>
	void format_va(String& outputText, const char*& formatText,T p1,TS... args);

	template<typename String,typename T>
	int ApplyToFormatStr(String& outputText, const char *& itr,T p1);

	namespace string_adaptor
	{
        // Function overloads for std::string and similiars 
		template <typename String>
		void reserve(String& self, int size) { self.reserve(size); }
		template <typename String>
		int append(String& self, int, const char *str) { self.append(str); return length(self); }
		template <typename String>
		int append(String& self, int, const char *str, int count) { self.append(str, count); return length(self); }
		template <typename String>
		int append(String& self, int, int count, char c) { self.append(count, c); return length(self); }
		template <typename String>
		int length(const String& self) { return static_cast<int>(self.length()); }
		template <typename String>
		char * at(String& self, int index) { return &self[index]; }
        template <typename String>
        void copy(String& self, const char *str) { self = str; }

        // Function overloads for fixed-size char arrays
		template <int N>
		void reserve(char (&self)[N], int size) {}
		template <int N>
		int append(char (&self)[N], int currentLength, const char *str)
        {
            assert(self[currentLength] == 0);
            assert(currentLength+strlen(str)<N);
            while(currentLength < N-1 && *str)
            {
                self[currentLength] = *str;
                ++str;
                ++currentLength;
            }
            self[currentLength] = 0;
            return currentLength;
        }
		template <int N>
		int append(char (&self)[N], int currentLength, const char *str, int count)
        {
            assert(self[currentLength] == 0);
            assert(currentLength+count<N);
            int end = std::min(N-1, count+currentLength);
            while(currentLength < end && *str)
            {
                self[currentLength] = *str;
                ++str;
                ++currentLength;
            }
            self[currentLength] = 0;
            return currentLength;
        }
		template <int N>
		int append(char (&self)[N], int currentLength, int count, char c)
        { 
            assert(self[currentLength] == 0);
            assert(currentLength+count<N);
            int end = std::min(N-1, count+currentLength);
            while(currentLength < end)
            {
                self[currentLength] = c;
                ++currentLength;
            }
            self[currentLength] = 0;
            return currentLength;
        }
		template <int N>
        int length(const char (&self)[N]) { return static_cast<int>(strlen(self)); }
		template <int N>
		char * at(char (&self)[N], int index)
        {
            assert(index<N);
            return &self[index];
        }
		template <int N>
        void copy(char (&self)[N], const char *str)
        {
#if _MSC_VER
            strcpy_s(self, N-1, str);
#else
            strlcpy(self, str, N-1);
#endif
        }
	}

	namespace detail
	{
		const int kGranulity = 32;	// This determines the reservation size for non-string arguments.
		static const double pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000,
																		10000000, 100000000, 1000000000 };
        // Integer render functions are inspired(optimized) from this: 
        // https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920
        int digits10(uint32_t v);
        int digits10(uint64_t v);

		template <typename String>
		int render(String& wstr, int currentLength, int32_t value, int width, int precision);
		template <typename String>
		int render(String& wstr, int currentLength,  uint32_t value, int width, int precision);
		template <typename String>
		int render(String& wstr, int currentLength,  int64_t value, int width, int precision);
		template <typename String>
		int render(String& wstr, int currentLength,  uint64_t value, int width, int precision);
		template <typename String>
		int render(String& wstr, int currentLength,  double value, int width, int precision);
		template <typename String>
		int render(String& wstr, int currentLength,  const char *value, int width, int precision);
		template <typename T>
		int size_enough(T value);
        template <>
        int size_enough(const char *value);
		void strreverse(char *begin, char *end);
	}
}


template<typename String,typename T>
int miniformat::ApplyToFormatStr(String& outputText, const char*& itr,T p1)
{
	char c = 0;
	int currentLength=0;
	while((c=*itr++))
	{
		if(c == '%')
		{
			if(*itr == '%')										/* "%%" */ 
			{
				currentLength = string_adaptor::append(outputText, currentLength, 1, '%');
				++itr;
			}
			else if(*itr >= '0' && *itr <= '9')					/* "%n" */
			{
				detail::render(outputText, currentLength, p1, 0, 6);
				++itr;
				return currentLength;
			}
			else if(*itr == '(' &&
				(*(itr+1) >= '0' && *(itr+1) <= '9') &&
				*(itr+2) == ')' &&
				(*(itr+3) >= '0' && *(itr+3) <= '9'))			/* %(w)n */
			{
				detail::render(outputText, currentLength, p1, *(itr+1)-'0', 6); 
				itr += 4;
				return currentLength;
			}
			else if(*itr == '(' &&
				*(itr+1) == '.' &&
				(*(itr+2) >= '0' && *(itr+2) <= '9') &&
				*(itr+3) == ')' &&
				(*(itr+4) >= '0' && *(itr+4) <= '9'))			/* "%(.p)n" */
			{
				detail::render(outputText, currentLength, p1, 0, *(itr+2)-'0');
				itr += 5;
				return currentLength;

			}
			else if(*itr == '(' &&
				(*(itr+1) >= '0' && *(itr+1) <= '9') &&
				*(itr+2) == '.' &&
				(*(itr+3) >= '0' && *(itr+3) <= '9') &&
				*(itr+4) == ')' &&
				(*(itr+5) >= '0' && *(itr+5) <= '9'))			/* %(w.p)n */
			{
				detail::render(outputText, currentLength, p1, *(itr+1)-'0', *(itr+3)-'0'); 
				itr += 6;
				return currentLength;
			}
			else
			{
				currentLength = string_adaptor::append(outputText, currentLength, 1, c);
				assert(!"An invalid format specifier given!");
			}
		}
		else
		{
			currentLength = string_adaptor::append(outputText, currentLength, 1, c);
		}
	}
	return currentLength;
}
template<typename... TS>
void miniformat::format_va(TS... args)
{

}


template<typename String,typename T,typename... TS>
void miniformat::format_va(String& outputText, const char*& formatText,T p1,TS... args)
{
	//ȡ1������������formatText,������Ҫ��ʽ���ĵط���ʹ�øò�����ʽ��
	int offset=ApplyToFormatStr(outputText,formatText,p1);
	format_va(outputText,formatText,args...);
}

template<typename T>
size_t calc_enouth_size_impl(const T& p)
{
	return miniformat::detail::size_enough(p);
}

template<typename T,typename... TS>
size_t calc_enouth_size_impl(const T& p,TS... args)
{
	return miniformat::detail::size_enough(p)+calc_enouth_size_impl(args...);
}

template<typename... TS>
size_t calc_enouth_size(const char *formatText,TS... args)
{
	size_t fmtsz=strlen(formatText);
	size_t vasz=calc_enouth_size_impl(args...);
	return fmtsz+vasz;
}

template<typename String,typename... TS>
void miniformat::format(String& outputText, const char *formatText,TS... args)
{
	//size_t len1=strlen(formatText);
	outputText="";
	size_t sz=calc_enouth_size(formatText,args...);
	string_adaptor::reserve(outputText, sz);

	format_va(outputText,formatText,args...);
	size_t len2=strlen(formatText);
	if(len2>0)
		string_adaptor::append(outputText, len2, formatText);

}

//[[[end]]]

inline void miniformat::detail::strreverse(char *begin, char *end)
{
	char aux;
	while(end > begin)
		aux = *end, *end-- = *begin, *begin++ = aux;
}

template <typename T>
int miniformat::detail::size_enough(T value)
{
	return kGranulity;
}

template <>
inline int miniformat::detail::size_enough(const char *value)
{
    return strlen(value);
}

inline int miniformat::detail::digits10(uint32_t v)
{
    static const uint32_t P01 = 10;
    static const uint32_t P02 = 100;
    static const uint32_t P03 = 1000;
    static const uint32_t P04 = 10000;
    static const uint32_t P05 = 100000;
    static const uint32_t P06 = 1000000;
    static const uint32_t P07 = 10000000;
    static const uint32_t P08 = 100000000;
    static const uint32_t P09 = 1000000000;

    if(v < P01) return 1;
    if(v < P02) return 2;
    if(v < P03) return 3;
    if(v < P08)
    {
        if(v < P06)
        {
            if(v < P04) return 4;
            return 5 + (v >= P05);
        }
        return 7 + (v >= P07);
    }
    return 9 + (v >= P09);
}

inline int miniformat::detail::digits10(uint64_t v)
{
    static const uint64_t P01 = 10;
    static const uint64_t P02 = 100;
    static const uint64_t P03 = 1000;
    static const uint64_t P04 = 10000;
    static const uint64_t P05 = 100000;
    static const uint64_t P06 = 1000000;
    static const uint64_t P07 = 10000000;
    static const uint64_t P08 = 100000000;
    static const uint64_t P09 = 1000000000;
    static const uint64_t P10 = 10000000000;
    static const uint64_t P11 = 100000000000;
    static const uint64_t P12 = 1000000000000;

    if(v < P01) return 1;
    if(v < P02) return 2;
    if(v < P03) return 3;
    if(v < P12)
    {
        if(v < P08)
        {
            if(v < P06)
            {
                if(v < P04) return 4;
                return 5 + (v >= P05);
            }
            return 7 + (v >= P07);
        }
        if(v < P10)
        {
            return 9 + (v >= P09);
        }
        return 11 + (v >= P11);
    }

    return 12 + digits10(v / P12);
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, int32_t value, int width, int)
{
    static const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";

	// Take care of sign.
	unsigned int uvalue = (value < 0) ? -value : value;

	int next = currentLength;
    const int length = digits10(uvalue);

	// Handle the 'width' parameter.
	int spaceCnt = width - length - (value < 0);
	if(spaceCnt > 0)
    {
		currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');
        next += spaceCnt;
    }

    if(value < 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, 1, '-');
        ++next;
    }

    currentLength = string_adaptor::append(wstr, currentLength, length, '0');
    next += length-1;

    while(uvalue >= 100)
    {
        const uint32_t i = (uvalue % 100) * 2;
        uvalue /= 100;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
        next -= 2;
    }

    // Handle last 1-2 digits.
    if(uvalue < 10)
    {
        *string_adaptor::at(wstr, next) = '0'+static_cast<char>(uvalue);
    }
    else
    {
        const uint32_t i = static_cast<uint32_t>(uvalue) * 2;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
    }

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, uint32_t value, int width, int)
{
    static const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";

	int next = currentLength;
    const int length = digits10(value);

	// Handle the 'width' parameter.
	int spaceCnt = width - length;
	if(spaceCnt > 0)
    {
		currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');
        next += spaceCnt;
    }

    currentLength = string_adaptor::append(wstr, currentLength, length, '0');
    next += length-1;

    while(value >= 100)
    {
        const uint32_t i = (value % 100) * 2;
        value /= 100;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
        next -= 2;
    }

    // Handle last 1-2 digits.
    if(value < 10)
    {
        *string_adaptor::at(wstr, next) = '0'+static_cast<char>(value);
    }
    else
    {
        const uint32_t i = static_cast<uint32_t>(value) * 2;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
    }

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, int64_t value, int width, int)
{
    static const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";

    // Take care of sign.
    uint64_t uvalue = (value < 0) ? -value : value;

	int next = currentLength;
    const int length = digits10(uvalue);

    // Handle the 'width' parameter.
    int spaceCnt = width - length - (value < 0);
    if(spaceCnt > 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');
        next += spaceCnt;
    }

    if(value < 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, 1, '-');
        ++next;
    }

    currentLength = string_adaptor::append(wstr, currentLength, length, '0');
    next += length-1;

    while(uvalue >= 100)
    {
        const uint64_t i = (uvalue % 100) * 2;
        uvalue /= 100;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
        next -= 2;
    }

    // Handle last 1-2 digits.
    if(uvalue < 10)
    {
        *string_adaptor::at(wstr, next) = '0'+static_cast<char>(uvalue);
    }
    else
    {
        const uint32_t i = static_cast<uint32_t>(uvalue) * 2;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
    }

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, uint64_t value, int width, int)
{
    static const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";

	int next = currentLength;
    const int length = digits10(value);

    // Handle the 'width' parameter.
    int spaceCnt = width - length;
    if(spaceCnt > 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');
        next += spaceCnt;
    }

    currentLength = string_adaptor::append(wstr, currentLength, length, '0');
    next += length-1;

    while(value >= 100)
    {
        const uint64_t i = (value % 100) * 2;
        value /= 100;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
        next -= 2;
    }

    // Handle last 1-2 digits.
    if(value < 10)
    {
        *string_adaptor::at(wstr, next) = '0'+static_cast<char>(value);
    }
    else
    {
        const uint32_t i = static_cast<uint32_t>(value) * 2;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
    }

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, double value, int width, int precision)
{
	int beginIndex = currentLength;
	/* Hacky test for NaN
	 * under -fast-math this won't work, but then you also won't
	 * have correct nan values anyways.  The alternative is
	 * to link with libmath (bad) or hack IEEE double bits (bad)
	 */
	if(!(value == value))		
	{
		// Handle the 'width' parameter.
		int spaceCnt = width - 3;
		if(spaceCnt > 0)
			currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');

		currentLength = string_adaptor::append(wstr, currentLength, 1, 'n'); 
		currentLength = string_adaptor::append(wstr, currentLength, 1, 'a'); 
		currentLength = string_adaptor::append(wstr, currentLength, 1, 'n');
		return currentLength;
	}

	/* if input is larger than thres_max, revert to exponential */
	const double thres_max = (double)(0x7FFFFFFF);

	double diff = 0.0;

	if(precision < 0) 
	{
		precision = 0;
	} 
	else if(precision > 9) 
	{
		/* precision of >= 10 can lead to overflow errors */
		precision = 9;
	}

	/* we'll work in positive values and deal with the
		negative sign issue later */
	int neg = 0;
	if(value < 0) 
	{
		neg = 1;
		value = -value;
	}

	int whole = (int)value;
	double tmp = (value - whole) * pow10[precision];
	uint32_t frac = (uint32_t)(tmp);
	diff = tmp - frac;

	if(diff > 0.5) 
	{
		++frac;
		/* handle rollover, e.g.  case 0.99 with precision 1 is 1.0  */
		if(frac >= pow10[precision]) 
		{
			frac = 0;
			++whole;
		}
	}
	else if(diff == 0.5 && ((frac == 0) || (frac & 1)))
	{
		/* if halfway, round up if odd, OR
			if last digit is 0.  That last part is strange */
		++frac;
	}

	/* for very large numbers switch back to native sprintf for exponentials.
		anyone want to write code to replace this? */
	/*
		 normal printf behavior is to print EVERY whole number digit
		which can be 100s of characters overflowing your buffers == bad
	*/
	if(value > thres_max)
	{
		const int kSomeEnoughSpace = 128;
		char buffer[kSomeEnoughSpace+1];
#if _MSC_VER
        sprintf_s(buffer, kSomeEnoughSpace, "%*e", width, neg ? -value : value);
#else
        snprintf(buffer, kSomeEnoughSpace, "%*e", width, neg ? -value : value);
#endif
		buffer[kSomeEnoughSpace] = 0;
		currentLength = string_adaptor::append(wstr, currentLength, buffer);
		return currentLength;
	}

	if(precision == 0)
	{
		diff = value - whole;
		if(diff > 0.5) 
		{
			/* greater than 0.5, round up, e.g. 1.6 -> 2 */
			++whole;
		} 
		else if(diff == 0.5 && (whole & 1)) 
		{
			/* exactly 0.5 and ODD, then round up */
			/* 1.5 -> 2, but 2.5 -> 2 */
			++whole;
		}
	} 
	else 
	{
		int count = precision;
		// now do fractional part, as an unsigned number
		do 
		{
			--count;
			currentLength = string_adaptor::append(wstr, currentLength, 1, (char)(48 + (frac % 10)));
		} 
		while((frac /= 10));
		// add extra 0s
		currentLength = string_adaptor::append(wstr, currentLength, count, '0');
		// add decimal
		currentLength = string_adaptor::append(wstr, currentLength, 1, '.');
	}

	// do whole part.
	// Take care of sign.
	// Conversion. Number is reversed.
	do 
	{
		currentLength = string_adaptor::append(wstr, currentLength, 1, (char)(48 + (whole % 10))); 
	}
	while((whole /= 10));
	if(neg) 
	{
		currentLength = string_adaptor::append(wstr, currentLength, 1, '-');
	}
    
	// Handle the 'width' parameter.
	int spaceCnt = width - (currentLength-beginIndex);
	if(spaceCnt > 0)
		currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');

	int endIndex = currentLength-1;
	strreverse(string_adaptor::at(wstr, beginIndex), string_adaptor::at(wstr, endIndex));

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, const char *value, int width, int)
{
	// Handle the 'width' parameter.
    int spaceCnt = width - strlen(value);
	if(spaceCnt > 0)
		currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');

	currentLength = string_adaptor::append(wstr, currentLength, value);

    return currentLength;
}
