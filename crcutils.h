#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

enum : uint32_t
{
    CRC32_MASK = 0xFFFFFFFF,
    CRC32_XOR = 0xFFFFFFFF,
    CRC32_SEED = CRC32_XOR,
};

enum : uint64_t
{
    CRC64_MASK = 0xFFFFFFFFFFFFFFFF,
    CRC64_XOR = 0xFFFFFFFFFFFFFFFF,
    CRC64_SEED = CRC64_XOR,
};

template <class T>
constexpr T bitReflectRecursive(T acc, T data, uint8_t bitCount)
{
    return bitCount > 0 ? bitReflectRecursive((acc << 1) | ((data >> 1) & 0x01), data >> 1, bitCount - 1) : acc;
}

template <class T>
constexpr T bitReflect(T data, uint8_t bitCount)
{
    return bitReflectRecursive(data & 0x01, data, bitCount - 1);
}
static_assert(bitReflect(0x04C11DB7u, 32) == 0xEDB88320, "Failed reflecting bits");

template <class T, uint8_t order, uint64_t POLYNOM>
class CrcTable
{
    static constexpr uint64_t ReveredPolynom = bitReflect(POLYNOM, order);

    template <class T>
    static constexpr T generateTableValueRecursive(T r, uint8_t i)
    {
        return i < 8 ? generateTableValueRecursive((r & 0x01) ? ((r >> 1) ^ ReveredPolynom) : (r >> 1), i + 1) : r;
    }

    static constexpr T init(uint8_t i)
    {
        return generateTableValueRecursive<T>(i, 0);
    }

public:
    constexpr CrcTable()
        : m_table
    {
          init(0),   init(1),   init(2),   init(3),   init(4),   init(5),   init(6),   init(7),
          init(8),   init(9),  init(10),  init(11),  init(12),  init(13),  init(14),  init(15),
         init(16),  init(17),  init(18),  init(19),  init(20),  init(21),  init(22),  init(23),
         init(24),  init(25),  init(26),  init(27),  init(28),  init(29),  init(30),  init(31),
         init(32),  init(33),  init(34),  init(35),  init(36),  init(37),  init(38),  init(39),
         init(40),  init(41),  init(42),  init(43),  init(44),  init(45),  init(46),  init(47),
         init(48),  init(49),  init(50),  init(51),  init(52),  init(53),  init(54),  init(55),
         init(56),  init(57),  init(58),  init(59),  init(60),  init(61),  init(62),  init(63),
         init(64),  init(65),  init(66),  init(67),  init(68),  init(69),  init(70),  init(71),
         init(72),  init(73),  init(74),  init(75),  init(76),  init(77),  init(78),  init(79),
         init(80),  init(81),  init(82),  init(83),  init(84),  init(85),  init(86),  init(87),
         init(88),  init(89),  init(90),  init(91),  init(92),  init(93),  init(94),  init(95),
         init(96),  init(97),  init(98),  init(99), init(100), init(101), init(102), init(103),
        init(104), init(105), init(106), init(107), init(108), init(109), init(110), init(111),
        init(112), init(113), init(114), init(115), init(116), init(117), init(118), init(119),
        init(120), init(121), init(122), init(123), init(124), init(125), init(126), init(127),
        init(128), init(129), init(130), init(131), init(132), init(133), init(134), init(135),
        init(136), init(137), init(138), init(139), init(140), init(141), init(142), init(143),
        init(144), init(145), init(146), init(147), init(148), init(149), init(150), init(151),
        init(152), init(153), init(154), init(155), init(156), init(157), init(158), init(159),
        init(160), init(161), init(162), init(163), init(164), init(165), init(166), init(167),
        init(168), init(169), init(170), init(171), init(172), init(173), init(174), init(175),
        init(176), init(177), init(178), init(179), init(180), init(181), init(182), init(183),
        init(184), init(185), init(186), init(187), init(188), init(189), init(190), init(191),
        init(192), init(193), init(194), init(195), init(196), init(197), init(198), init(199),
        init(200), init(201), init(202), init(203), init(204), init(205), init(206), init(207),
        init(208), init(209), init(210), init(211), init(212), init(213), init(214), init(215),
        init(216), init(217), init(218), init(219), init(220), init(221), init(222), init(223),
        init(224), init(225), init(226), init(227), init(228), init(229), init(230), init(231),
        init(232), init(233), init(234), init(235), init(236), init(237), init(238), init(239),
        init(240), init(241), init(242), init(243), init(244), init(245), init(246), init(247),
        init(248), init(249), init(250), init(251), init(252), init(253), init(254), init(255),
    }
    {
    }

    constexpr T operator [](int index) const
    {
        return m_table[index];
    }

private:
    T const m_table[256];
};

constexpr CrcTable<uint32_t, 32, 0x04C11DB7> Crc32Ansi;
constexpr CrcTable<uint64_t, 64, 0x42F0E1EBA9EA3693> Crc64ECMA;

constexpr uint32_t calculateCrc32(uint32_t crc, const char* a, size_t i, size_t n)
{
    return i < n ? calculateCrc32((crc >> 8) ^ Crc32Ansi[(crc & 0xFF) ^ a[i]], a, i + 1, n) : crc;
}

template <size_t N>
constexpr uint32_t crc32(const char(&a)[N])
{
    return calculateCrc32(CRC32_SEED, a, 0, N-1) ^ CRC32_XOR;
}
static_assert(crc32("123456789") == 0xCBF43926U, "Incorrect CRC-32!");

static uint32_t crc32(const char* cstr)
{
    return calculateCrc32(CRC32_SEED, cstr, 0, strlen(cstr)) ^ CRC32_XOR;
}

constexpr uint32_t operator "" _crc32(const char* a, std::size_t n)
{
    return calculateCrc32(CRC32_SEED, a, 0, n) ^ CRC32_XOR;
}
static_assert("123456789"_crc32 == 0xCBF43926, "Incorrect CRC-32!");

constexpr uint64_t calculateCrc64(uint64_t crc, const char* a, size_t i, size_t n)
{
    return i < n ? calculateCrc64((crc >> 8) ^ Crc64ECMA[(crc & 0xFF) ^ a[i]], a, i + 1, n) : crc;
}

template <size_t N>
constexpr uint64_t crc64(const char(&a)[N])
{
    return calculateCrc64(CRC64_SEED, a, 0, N-1) ^ CRC64_XOR;
}
static_assert(crc64("123456789") == 0x995DC9BBDF1939FA, "Incorrect CRC-64!");

static uint64_t crc64(const char* cstr)
{
    return calculateCrc64(CRC64_SEED, cstr, 0, strlen(cstr)) ^ CRC64_XOR;
}

constexpr uint64_t operator "" _crc64(const char* a, std::size_t n)
{
    return calculateCrc64(CRC64_SEED, a, 0, n) ^ CRC64_XOR;
}
static_assert("123456789"_crc64 == 0x995DC9BBDF1939FA, "Incorrect CRC-64!");
