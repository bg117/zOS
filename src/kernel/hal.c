/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <syslvl/core.h>
#include <syslvl/gdt.h>
#include <syslvl/hal.h>
#include <syslvl/idt.h>
#include <syslvl/video.h>

#include <misc/bit_macros.h>

static struct gdt_entry      _gdt[3];
static struct gdt_descriptor _gdt_desc;

static struct idt_entry      _idt[256];
static struct idt_descriptor _idt_desc;

static void _halinitgdt(void);
static void _halinitisr(void);

struct exception
{
    uint32_t data_segment;
    uint32_t destination;
    uint32_t source;
    uint32_t stack_top_ptr;    // EBP
    uint32_t stack_bottom_ptr; // ESP
    uint32_t base;
    uint32_t data;
    uint32_t counter;
    uint32_t accumulator;

    // CPU-managed
    uint32_t interrupt_index;
    uint32_t error_code;
    uint32_t instruction_ptr;
    uint32_t code_segment;
    uint32_t flags;
    uint32_t stack_bottom_ptr_cpu;
    uint32_t stack_segment;
};

void __defexcept(struct exception *regs) __attribute__((noreturn));

#pragma region ISR stubs
extern void __attribute__((cdecl)) __isr_stub_0(void);
extern void __attribute__((cdecl)) __isr_stub_1(void);
extern void __attribute__((cdecl)) __isr_stub_2(void);
extern void __attribute__((cdecl)) __isr_stub_3(void);
extern void __attribute__((cdecl)) __isr_stub_4(void);
extern void __attribute__((cdecl)) __isr_stub_5(void);
extern void __attribute__((cdecl)) __isr_stub_6(void);
extern void __attribute__((cdecl)) __isr_stub_7(void);
extern void __attribute__((cdecl)) __isr_stub_8(void);
extern void __attribute__((cdecl)) __isr_stub_9(void);
extern void __attribute__((cdecl)) __isr_stub_10(void);
extern void __attribute__((cdecl)) __isr_stub_11(void);
extern void __attribute__((cdecl)) __isr_stub_12(void);
extern void __attribute__((cdecl)) __isr_stub_13(void);
extern void __attribute__((cdecl)) __isr_stub_14(void);
extern void __attribute__((cdecl)) __isr_stub_15(void);
extern void __attribute__((cdecl)) __isr_stub_16(void);
extern void __attribute__((cdecl)) __isr_stub_17(void);
extern void __attribute__((cdecl)) __isr_stub_18(void);
extern void __attribute__((cdecl)) __isr_stub_19(void);
extern void __attribute__((cdecl)) __isr_stub_20(void);
extern void __attribute__((cdecl)) __isr_stub_21(void);
extern void __attribute__((cdecl)) __isr_stub_22(void);
extern void __attribute__((cdecl)) __isr_stub_23(void);
extern void __attribute__((cdecl)) __isr_stub_24(void);
extern void __attribute__((cdecl)) __isr_stub_25(void);
extern void __attribute__((cdecl)) __isr_stub_26(void);
extern void __attribute__((cdecl)) __isr_stub_27(void);
extern void __attribute__((cdecl)) __isr_stub_28(void);
extern void __attribute__((cdecl)) __isr_stub_29(void);
extern void __attribute__((cdecl)) __isr_stub_30(void);
extern void __attribute__((cdecl)) __isr_stub_31(void);
extern void __attribute__((cdecl)) __isr_stub_32(void);
extern void __attribute__((cdecl)) __isr_stub_33(void);
extern void __attribute__((cdecl)) __isr_stub_34(void);
extern void __attribute__((cdecl)) __isr_stub_35(void);
extern void __attribute__((cdecl)) __isr_stub_36(void);
extern void __attribute__((cdecl)) __isr_stub_37(void);
extern void __attribute__((cdecl)) __isr_stub_38(void);
extern void __attribute__((cdecl)) __isr_stub_39(void);
extern void __attribute__((cdecl)) __isr_stub_40(void);
extern void __attribute__((cdecl)) __isr_stub_41(void);
extern void __attribute__((cdecl)) __isr_stub_42(void);
extern void __attribute__((cdecl)) __isr_stub_43(void);
extern void __attribute__((cdecl)) __isr_stub_44(void);
extern void __attribute__((cdecl)) __isr_stub_45(void);
extern void __attribute__((cdecl)) __isr_stub_46(void);
extern void __attribute__((cdecl)) __isr_stub_47(void);
extern void __attribute__((cdecl)) __isr_stub_48(void);
extern void __attribute__((cdecl)) __isr_stub_49(void);
extern void __attribute__((cdecl)) __isr_stub_50(void);
extern void __attribute__((cdecl)) __isr_stub_51(void);
extern void __attribute__((cdecl)) __isr_stub_52(void);
extern void __attribute__((cdecl)) __isr_stub_53(void);
extern void __attribute__((cdecl)) __isr_stub_54(void);
extern void __attribute__((cdecl)) __isr_stub_55(void);
extern void __attribute__((cdecl)) __isr_stub_56(void);
extern void __attribute__((cdecl)) __isr_stub_57(void);
extern void __attribute__((cdecl)) __isr_stub_58(void);
extern void __attribute__((cdecl)) __isr_stub_59(void);
extern void __attribute__((cdecl)) __isr_stub_60(void);
extern void __attribute__((cdecl)) __isr_stub_61(void);
extern void __attribute__((cdecl)) __isr_stub_62(void);
extern void __attribute__((cdecl)) __isr_stub_63(void);
extern void __attribute__((cdecl)) __isr_stub_64(void);
extern void __attribute__((cdecl)) __isr_stub_65(void);
extern void __attribute__((cdecl)) __isr_stub_66(void);
extern void __attribute__((cdecl)) __isr_stub_67(void);
extern void __attribute__((cdecl)) __isr_stub_68(void);
extern void __attribute__((cdecl)) __isr_stub_69(void);
extern void __attribute__((cdecl)) __isr_stub_70(void);
extern void __attribute__((cdecl)) __isr_stub_71(void);
extern void __attribute__((cdecl)) __isr_stub_72(void);
extern void __attribute__((cdecl)) __isr_stub_73(void);
extern void __attribute__((cdecl)) __isr_stub_74(void);
extern void __attribute__((cdecl)) __isr_stub_75(void);
extern void __attribute__((cdecl)) __isr_stub_76(void);
extern void __attribute__((cdecl)) __isr_stub_77(void);
extern void __attribute__((cdecl)) __isr_stub_78(void);
extern void __attribute__((cdecl)) __isr_stub_79(void);
extern void __attribute__((cdecl)) __isr_stub_80(void);
extern void __attribute__((cdecl)) __isr_stub_81(void);
extern void __attribute__((cdecl)) __isr_stub_82(void);
extern void __attribute__((cdecl)) __isr_stub_83(void);
extern void __attribute__((cdecl)) __isr_stub_84(void);
extern void __attribute__((cdecl)) __isr_stub_85(void);
extern void __attribute__((cdecl)) __isr_stub_86(void);
extern void __attribute__((cdecl)) __isr_stub_87(void);
extern void __attribute__((cdecl)) __isr_stub_88(void);
extern void __attribute__((cdecl)) __isr_stub_89(void);
extern void __attribute__((cdecl)) __isr_stub_90(void);
extern void __attribute__((cdecl)) __isr_stub_91(void);
extern void __attribute__((cdecl)) __isr_stub_92(void);
extern void __attribute__((cdecl)) __isr_stub_93(void);
extern void __attribute__((cdecl)) __isr_stub_94(void);
extern void __attribute__((cdecl)) __isr_stub_95(void);
extern void __attribute__((cdecl)) __isr_stub_96(void);
extern void __attribute__((cdecl)) __isr_stub_97(void);
extern void __attribute__((cdecl)) __isr_stub_98(void);
extern void __attribute__((cdecl)) __isr_stub_99(void);
extern void __attribute__((cdecl)) __isr_stub_100(void);
extern void __attribute__((cdecl)) __isr_stub_101(void);
extern void __attribute__((cdecl)) __isr_stub_102(void);
extern void __attribute__((cdecl)) __isr_stub_103(void);
extern void __attribute__((cdecl)) __isr_stub_104(void);
extern void __attribute__((cdecl)) __isr_stub_105(void);
extern void __attribute__((cdecl)) __isr_stub_106(void);
extern void __attribute__((cdecl)) __isr_stub_107(void);
extern void __attribute__((cdecl)) __isr_stub_108(void);
extern void __attribute__((cdecl)) __isr_stub_109(void);
extern void __attribute__((cdecl)) __isr_stub_110(void);
extern void __attribute__((cdecl)) __isr_stub_111(void);
extern void __attribute__((cdecl)) __isr_stub_112(void);
extern void __attribute__((cdecl)) __isr_stub_113(void);
extern void __attribute__((cdecl)) __isr_stub_114(void);
extern void __attribute__((cdecl)) __isr_stub_115(void);
extern void __attribute__((cdecl)) __isr_stub_116(void);
extern void __attribute__((cdecl)) __isr_stub_117(void);
extern void __attribute__((cdecl)) __isr_stub_118(void);
extern void __attribute__((cdecl)) __isr_stub_119(void);
extern void __attribute__((cdecl)) __isr_stub_120(void);
extern void __attribute__((cdecl)) __isr_stub_121(void);
extern void __attribute__((cdecl)) __isr_stub_122(void);
extern void __attribute__((cdecl)) __isr_stub_123(void);
extern void __attribute__((cdecl)) __isr_stub_124(void);
extern void __attribute__((cdecl)) __isr_stub_125(void);
extern void __attribute__((cdecl)) __isr_stub_126(void);
extern void __attribute__((cdecl)) __isr_stub_127(void);
extern void __attribute__((cdecl)) __isr_stub_128(void);
extern void __attribute__((cdecl)) __isr_stub_129(void);
extern void __attribute__((cdecl)) __isr_stub_130(void);
extern void __attribute__((cdecl)) __isr_stub_131(void);
extern void __attribute__((cdecl)) __isr_stub_132(void);
extern void __attribute__((cdecl)) __isr_stub_133(void);
extern void __attribute__((cdecl)) __isr_stub_134(void);
extern void __attribute__((cdecl)) __isr_stub_135(void);
extern void __attribute__((cdecl)) __isr_stub_136(void);
extern void __attribute__((cdecl)) __isr_stub_137(void);
extern void __attribute__((cdecl)) __isr_stub_138(void);
extern void __attribute__((cdecl)) __isr_stub_139(void);
extern void __attribute__((cdecl)) __isr_stub_140(void);
extern void __attribute__((cdecl)) __isr_stub_141(void);
extern void __attribute__((cdecl)) __isr_stub_142(void);
extern void __attribute__((cdecl)) __isr_stub_143(void);
extern void __attribute__((cdecl)) __isr_stub_144(void);
extern void __attribute__((cdecl)) __isr_stub_145(void);
extern void __attribute__((cdecl)) __isr_stub_146(void);
extern void __attribute__((cdecl)) __isr_stub_147(void);
extern void __attribute__((cdecl)) __isr_stub_148(void);
extern void __attribute__((cdecl)) __isr_stub_149(void);
extern void __attribute__((cdecl)) __isr_stub_150(void);
extern void __attribute__((cdecl)) __isr_stub_151(void);
extern void __attribute__((cdecl)) __isr_stub_152(void);
extern void __attribute__((cdecl)) __isr_stub_153(void);
extern void __attribute__((cdecl)) __isr_stub_154(void);
extern void __attribute__((cdecl)) __isr_stub_155(void);
extern void __attribute__((cdecl)) __isr_stub_156(void);
extern void __attribute__((cdecl)) __isr_stub_157(void);
extern void __attribute__((cdecl)) __isr_stub_158(void);
extern void __attribute__((cdecl)) __isr_stub_159(void);
extern void __attribute__((cdecl)) __isr_stub_160(void);
extern void __attribute__((cdecl)) __isr_stub_161(void);
extern void __attribute__((cdecl)) __isr_stub_162(void);
extern void __attribute__((cdecl)) __isr_stub_163(void);
extern void __attribute__((cdecl)) __isr_stub_164(void);
extern void __attribute__((cdecl)) __isr_stub_165(void);
extern void __attribute__((cdecl)) __isr_stub_166(void);
extern void __attribute__((cdecl)) __isr_stub_167(void);
extern void __attribute__((cdecl)) __isr_stub_168(void);
extern void __attribute__((cdecl)) __isr_stub_169(void);
extern void __attribute__((cdecl)) __isr_stub_170(void);
extern void __attribute__((cdecl)) __isr_stub_171(void);
extern void __attribute__((cdecl)) __isr_stub_172(void);
extern void __attribute__((cdecl)) __isr_stub_173(void);
extern void __attribute__((cdecl)) __isr_stub_174(void);
extern void __attribute__((cdecl)) __isr_stub_175(void);
extern void __attribute__((cdecl)) __isr_stub_176(void);
extern void __attribute__((cdecl)) __isr_stub_177(void);
extern void __attribute__((cdecl)) __isr_stub_178(void);
extern void __attribute__((cdecl)) __isr_stub_179(void);
extern void __attribute__((cdecl)) __isr_stub_180(void);
extern void __attribute__((cdecl)) __isr_stub_181(void);
extern void __attribute__((cdecl)) __isr_stub_182(void);
extern void __attribute__((cdecl)) __isr_stub_183(void);
extern void __attribute__((cdecl)) __isr_stub_184(void);
extern void __attribute__((cdecl)) __isr_stub_185(void);
extern void __attribute__((cdecl)) __isr_stub_186(void);
extern void __attribute__((cdecl)) __isr_stub_187(void);
extern void __attribute__((cdecl)) __isr_stub_188(void);
extern void __attribute__((cdecl)) __isr_stub_189(void);
extern void __attribute__((cdecl)) __isr_stub_190(void);
extern void __attribute__((cdecl)) __isr_stub_191(void);
extern void __attribute__((cdecl)) __isr_stub_192(void);
extern void __attribute__((cdecl)) __isr_stub_193(void);
extern void __attribute__((cdecl)) __isr_stub_194(void);
extern void __attribute__((cdecl)) __isr_stub_195(void);
extern void __attribute__((cdecl)) __isr_stub_196(void);
extern void __attribute__((cdecl)) __isr_stub_197(void);
extern void __attribute__((cdecl)) __isr_stub_198(void);
extern void __attribute__((cdecl)) __isr_stub_199(void);
extern void __attribute__((cdecl)) __isr_stub_200(void);
extern void __attribute__((cdecl)) __isr_stub_201(void);
extern void __attribute__((cdecl)) __isr_stub_202(void);
extern void __attribute__((cdecl)) __isr_stub_203(void);
extern void __attribute__((cdecl)) __isr_stub_204(void);
extern void __attribute__((cdecl)) __isr_stub_205(void);
extern void __attribute__((cdecl)) __isr_stub_206(void);
extern void __attribute__((cdecl)) __isr_stub_207(void);
extern void __attribute__((cdecl)) __isr_stub_208(void);
extern void __attribute__((cdecl)) __isr_stub_209(void);
extern void __attribute__((cdecl)) __isr_stub_210(void);
extern void __attribute__((cdecl)) __isr_stub_211(void);
extern void __attribute__((cdecl)) __isr_stub_212(void);
extern void __attribute__((cdecl)) __isr_stub_213(void);
extern void __attribute__((cdecl)) __isr_stub_214(void);
extern void __attribute__((cdecl)) __isr_stub_215(void);
extern void __attribute__((cdecl)) __isr_stub_216(void);
extern void __attribute__((cdecl)) __isr_stub_217(void);
extern void __attribute__((cdecl)) __isr_stub_218(void);
extern void __attribute__((cdecl)) __isr_stub_219(void);
extern void __attribute__((cdecl)) __isr_stub_220(void);
extern void __attribute__((cdecl)) __isr_stub_221(void);
extern void __attribute__((cdecl)) __isr_stub_222(void);
extern void __attribute__((cdecl)) __isr_stub_223(void);
extern void __attribute__((cdecl)) __isr_stub_224(void);
extern void __attribute__((cdecl)) __isr_stub_225(void);
extern void __attribute__((cdecl)) __isr_stub_226(void);
extern void __attribute__((cdecl)) __isr_stub_227(void);
extern void __attribute__((cdecl)) __isr_stub_228(void);
extern void __attribute__((cdecl)) __isr_stub_229(void);
extern void __attribute__((cdecl)) __isr_stub_230(void);
extern void __attribute__((cdecl)) __isr_stub_231(void);
extern void __attribute__((cdecl)) __isr_stub_232(void);
extern void __attribute__((cdecl)) __isr_stub_233(void);
extern void __attribute__((cdecl)) __isr_stub_234(void);
extern void __attribute__((cdecl)) __isr_stub_235(void);
extern void __attribute__((cdecl)) __isr_stub_236(void);
extern void __attribute__((cdecl)) __isr_stub_237(void);
extern void __attribute__((cdecl)) __isr_stub_238(void);
extern void __attribute__((cdecl)) __isr_stub_239(void);
extern void __attribute__((cdecl)) __isr_stub_240(void);
extern void __attribute__((cdecl)) __isr_stub_241(void);
extern void __attribute__((cdecl)) __isr_stub_242(void);
extern void __attribute__((cdecl)) __isr_stub_243(void);
extern void __attribute__((cdecl)) __isr_stub_244(void);
extern void __attribute__((cdecl)) __isr_stub_245(void);
extern void __attribute__((cdecl)) __isr_stub_246(void);
extern void __attribute__((cdecl)) __isr_stub_247(void);
extern void __attribute__((cdecl)) __isr_stub_248(void);
extern void __attribute__((cdecl)) __isr_stub_249(void);
extern void __attribute__((cdecl)) __isr_stub_250(void);
extern void __attribute__((cdecl)) __isr_stub_251(void);
extern void __attribute__((cdecl)) __isr_stub_252(void);
extern void __attribute__((cdecl)) __isr_stub_253(void);
extern void __attribute__((cdecl)) __isr_stub_254(void);
extern void __attribute__((cdecl)) __isr_stub_255(void);
#pragma endregion

void halinit(void)
{
    _halinitgdt();
    gdtdescld(&_gdt_desc, 0x08, 0x10);

    _halinitisr();
    idtdescld(&_idt_desc);

    coresetint();
}

void _halinitgdt()
{
    _gdt[0] = gdtmkentry(0x0000, 0x0000, 0x0000, 0x0000);
    _gdt[1] = gdtmkentry(0xFFFFF,
                         0x0000,
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_CODE
                             | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                         GDT_OF_GRANULARITY | GDT_OF_32BIT);
    _gdt[2] = gdtmkentry(0xFFFFF,
                         0x0000,
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_DATA
                             | GDT_AX_DATA_WRITABLE,
                         GDT_OF_GRANULARITY | GDT_OF_32BIT);

    gdtdescinit(&_gdt_desc, _gdt, sizeof _gdt / sizeof *_gdt);
}

void _halinitisr()
{
    // oh, and, before you ask, this part was generated by a shell for-loop ;)
#pragma region ISR initialization
    _idt[0] = idtmkentry(
        __isr_stub_0, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[1] = idtmkentry(
        __isr_stub_1, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[2] = idtmkentry(
        __isr_stub_2, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[3] = idtmkentry(
        __isr_stub_3, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[4] = idtmkentry(
        __isr_stub_4, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[5] = idtmkentry(
        __isr_stub_5, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[6] = idtmkentry(
        __isr_stub_6, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[7] = idtmkentry(
        __isr_stub_7, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[8] = idtmkentry(
        __isr_stub_8, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[9] = idtmkentry(
        __isr_stub_9, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[10] = idtmkentry(
        __isr_stub_10, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[11] = idtmkentry(
        __isr_stub_11, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[12] = idtmkentry(
        __isr_stub_12, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[13] = idtmkentry(
        __isr_stub_13, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[14] = idtmkentry(
        __isr_stub_14, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[15] = idtmkentry(
        __isr_stub_15, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[16] = idtmkentry(
        __isr_stub_16, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[17] = idtmkentry(
        __isr_stub_17, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[18] = idtmkentry(
        __isr_stub_18, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[19] = idtmkentry(
        __isr_stub_19, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[20] = idtmkentry(
        __isr_stub_20, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[21] = idtmkentry(
        __isr_stub_21, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[22] = idtmkentry(
        __isr_stub_22, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[23] = idtmkentry(
        __isr_stub_23, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[24] = idtmkentry(
        __isr_stub_24, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[25] = idtmkentry(
        __isr_stub_25, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[26] = idtmkentry(
        __isr_stub_26, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[27] = idtmkentry(
        __isr_stub_27, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[28] = idtmkentry(
        __isr_stub_28, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[29] = idtmkentry(
        __isr_stub_29, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[30] = idtmkentry(
        __isr_stub_30, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[31] = idtmkentry(
        __isr_stub_31, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[32] = idtmkentry(
        __isr_stub_32, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[33] = idtmkentry(
        __isr_stub_33, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[34] = idtmkentry(
        __isr_stub_34, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[35] = idtmkentry(
        __isr_stub_35, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[36] = idtmkentry(
        __isr_stub_36, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[37] = idtmkentry(
        __isr_stub_37, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[38] = idtmkentry(
        __isr_stub_38, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[39] = idtmkentry(
        __isr_stub_39, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[40] = idtmkentry(
        __isr_stub_40, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[41] = idtmkentry(
        __isr_stub_41, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[42] = idtmkentry(
        __isr_stub_42, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[43] = idtmkentry(
        __isr_stub_43, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[44] = idtmkentry(
        __isr_stub_44, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[45] = idtmkentry(
        __isr_stub_45, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[46] = idtmkentry(
        __isr_stub_46, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[47] = idtmkentry(
        __isr_stub_47, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[48] = idtmkentry(
        __isr_stub_48, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[49] = idtmkentry(
        __isr_stub_49, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[50] = idtmkentry(
        __isr_stub_50, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[51] = idtmkentry(
        __isr_stub_51, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[52] = idtmkentry(
        __isr_stub_52, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[53] = idtmkentry(
        __isr_stub_53, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[54] = idtmkentry(
        __isr_stub_54, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[55] = idtmkentry(
        __isr_stub_55, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[56] = idtmkentry(
        __isr_stub_56, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[57] = idtmkentry(
        __isr_stub_57, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[58] = idtmkentry(
        __isr_stub_58, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[59] = idtmkentry(
        __isr_stub_59, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[60] = idtmkentry(
        __isr_stub_60, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[61] = idtmkentry(
        __isr_stub_61, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[62] = idtmkentry(
        __isr_stub_62, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[63] = idtmkentry(
        __isr_stub_63, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[64] = idtmkentry(
        __isr_stub_64, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[65] = idtmkentry(
        __isr_stub_65, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[66] = idtmkentry(
        __isr_stub_66, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[67] = idtmkentry(
        __isr_stub_67, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[68] = idtmkentry(
        __isr_stub_68, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[69] = idtmkentry(
        __isr_stub_69, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[70] = idtmkentry(
        __isr_stub_70, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[71] = idtmkentry(
        __isr_stub_71, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[72] = idtmkentry(
        __isr_stub_72, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[73] = idtmkentry(
        __isr_stub_73, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[74] = idtmkentry(
        __isr_stub_74, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[75] = idtmkentry(
        __isr_stub_75, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[76] = idtmkentry(
        __isr_stub_76, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[77] = idtmkentry(
        __isr_stub_77, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[78] = idtmkentry(
        __isr_stub_78, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[79] = idtmkentry(
        __isr_stub_79, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[80] = idtmkentry(
        __isr_stub_80, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[81] = idtmkentry(
        __isr_stub_81, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[82] = idtmkentry(
        __isr_stub_82, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[83] = idtmkentry(
        __isr_stub_83, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[84] = idtmkentry(
        __isr_stub_84, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[85] = idtmkentry(
        __isr_stub_85, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[86] = idtmkentry(
        __isr_stub_86, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[87] = idtmkentry(
        __isr_stub_87, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[88] = idtmkentry(
        __isr_stub_88, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[89] = idtmkentry(
        __isr_stub_89, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[90] = idtmkentry(
        __isr_stub_90, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[91] = idtmkentry(
        __isr_stub_91, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[92] = idtmkentry(
        __isr_stub_92, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[93] = idtmkentry(
        __isr_stub_93, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[94] = idtmkentry(
        __isr_stub_94, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[95] = idtmkentry(
        __isr_stub_95, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[96] = idtmkentry(
        __isr_stub_96, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[97] = idtmkentry(
        __isr_stub_97, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[98] = idtmkentry(
        __isr_stub_98, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[99] = idtmkentry(
        __isr_stub_99, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[100] = idtmkentry(
        __isr_stub_100, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[101] = idtmkentry(
        __isr_stub_101, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[102] = idtmkentry(
        __isr_stub_102, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[103] = idtmkentry(
        __isr_stub_103, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[104] = idtmkentry(
        __isr_stub_104, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[105] = idtmkentry(
        __isr_stub_105, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[106] = idtmkentry(
        __isr_stub_106, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[107] = idtmkentry(
        __isr_stub_107, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[108] = idtmkentry(
        __isr_stub_108, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[109] = idtmkentry(
        __isr_stub_109, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[110] = idtmkentry(
        __isr_stub_110, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[111] = idtmkentry(
        __isr_stub_111, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[112] = idtmkentry(
        __isr_stub_112, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[113] = idtmkentry(
        __isr_stub_113, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[114] = idtmkentry(
        __isr_stub_114, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[115] = idtmkentry(
        __isr_stub_115, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[116] = idtmkentry(
        __isr_stub_116, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[117] = idtmkentry(
        __isr_stub_117, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[118] = idtmkentry(
        __isr_stub_118, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[119] = idtmkentry(
        __isr_stub_119, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[120] = idtmkentry(
        __isr_stub_120, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[121] = idtmkentry(
        __isr_stub_121, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[122] = idtmkentry(
        __isr_stub_122, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[123] = idtmkentry(
        __isr_stub_123, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[124] = idtmkentry(
        __isr_stub_124, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[125] = idtmkentry(
        __isr_stub_125, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[126] = idtmkentry(
        __isr_stub_126, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[127] = idtmkentry(
        __isr_stub_127, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[128] = idtmkentry(
        __isr_stub_128, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[129] = idtmkentry(
        __isr_stub_129, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[130] = idtmkentry(
        __isr_stub_130, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[131] = idtmkentry(
        __isr_stub_131, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[132] = idtmkentry(
        __isr_stub_132, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[133] = idtmkentry(
        __isr_stub_133, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[134] = idtmkentry(
        __isr_stub_134, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[135] = idtmkentry(
        __isr_stub_135, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[136] = idtmkentry(
        __isr_stub_136, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[137] = idtmkentry(
        __isr_stub_137, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[138] = idtmkentry(
        __isr_stub_138, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[139] = idtmkentry(
        __isr_stub_139, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[140] = idtmkentry(
        __isr_stub_140, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[141] = idtmkentry(
        __isr_stub_141, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[142] = idtmkentry(
        __isr_stub_142, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[143] = idtmkentry(
        __isr_stub_143, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[144] = idtmkentry(
        __isr_stub_144, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[145] = idtmkentry(
        __isr_stub_145, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[146] = idtmkentry(
        __isr_stub_146, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[147] = idtmkentry(
        __isr_stub_147, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[148] = idtmkentry(
        __isr_stub_148, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[149] = idtmkentry(
        __isr_stub_149, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[150] = idtmkentry(
        __isr_stub_150, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[151] = idtmkentry(
        __isr_stub_151, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[152] = idtmkentry(
        __isr_stub_152, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[153] = idtmkentry(
        __isr_stub_153, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[154] = idtmkentry(
        __isr_stub_154, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[155] = idtmkentry(
        __isr_stub_155, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[156] = idtmkentry(
        __isr_stub_156, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[157] = idtmkentry(
        __isr_stub_157, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[158] = idtmkentry(
        __isr_stub_158, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[159] = idtmkentry(
        __isr_stub_159, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[160] = idtmkentry(
        __isr_stub_160, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[161] = idtmkentry(
        __isr_stub_161, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[162] = idtmkentry(
        __isr_stub_162, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[163] = idtmkentry(
        __isr_stub_163, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[164] = idtmkentry(
        __isr_stub_164, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[165] = idtmkentry(
        __isr_stub_165, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[166] = idtmkentry(
        __isr_stub_166, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[167] = idtmkentry(
        __isr_stub_167, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[168] = idtmkentry(
        __isr_stub_168, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[169] = idtmkentry(
        __isr_stub_169, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[170] = idtmkentry(
        __isr_stub_170, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[171] = idtmkentry(
        __isr_stub_171, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[172] = idtmkentry(
        __isr_stub_172, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[173] = idtmkentry(
        __isr_stub_173, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[174] = idtmkentry(
        __isr_stub_174, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[175] = idtmkentry(
        __isr_stub_175, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[176] = idtmkentry(
        __isr_stub_176, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[177] = idtmkentry(
        __isr_stub_177, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[178] = idtmkentry(
        __isr_stub_178, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[179] = idtmkentry(
        __isr_stub_179, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[180] = idtmkentry(
        __isr_stub_180, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[181] = idtmkentry(
        __isr_stub_181, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[182] = idtmkentry(
        __isr_stub_182, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[183] = idtmkentry(
        __isr_stub_183, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[184] = idtmkentry(
        __isr_stub_184, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[185] = idtmkentry(
        __isr_stub_185, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[186] = idtmkentry(
        __isr_stub_186, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[187] = idtmkentry(
        __isr_stub_187, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[188] = idtmkentry(
        __isr_stub_188, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[189] = idtmkentry(
        __isr_stub_189, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[190] = idtmkentry(
        __isr_stub_190, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[191] = idtmkentry(
        __isr_stub_191, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[192] = idtmkentry(
        __isr_stub_192, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[193] = idtmkentry(
        __isr_stub_193, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[194] = idtmkentry(
        __isr_stub_194, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[195] = idtmkentry(
        __isr_stub_195, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[196] = idtmkentry(
        __isr_stub_196, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[197] = idtmkentry(
        __isr_stub_197, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[198] = idtmkentry(
        __isr_stub_198, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[199] = idtmkentry(
        __isr_stub_199, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[200] = idtmkentry(
        __isr_stub_200, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[201] = idtmkentry(
        __isr_stub_201, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[202] = idtmkentry(
        __isr_stub_202, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[203] = idtmkentry(
        __isr_stub_203, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[204] = idtmkentry(
        __isr_stub_204, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[205] = idtmkentry(
        __isr_stub_205, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[206] = idtmkentry(
        __isr_stub_206, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[207] = idtmkentry(
        __isr_stub_207, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[208] = idtmkentry(
        __isr_stub_208, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[209] = idtmkentry(
        __isr_stub_209, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[210] = idtmkentry(
        __isr_stub_210, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[211] = idtmkentry(
        __isr_stub_211, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[212] = idtmkentry(
        __isr_stub_212, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[213] = idtmkentry(
        __isr_stub_213, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[214] = idtmkentry(
        __isr_stub_214, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[215] = idtmkentry(
        __isr_stub_215, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[216] = idtmkentry(
        __isr_stub_216, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[217] = idtmkentry(
        __isr_stub_217, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[218] = idtmkentry(
        __isr_stub_218, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[219] = idtmkentry(
        __isr_stub_219, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[220] = idtmkentry(
        __isr_stub_220, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[221] = idtmkentry(
        __isr_stub_221, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[222] = idtmkentry(
        __isr_stub_222, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[223] = idtmkentry(
        __isr_stub_223, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[224] = idtmkentry(
        __isr_stub_224, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[225] = idtmkentry(
        __isr_stub_225, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[226] = idtmkentry(
        __isr_stub_226, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[227] = idtmkentry(
        __isr_stub_227, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[228] = idtmkentry(
        __isr_stub_228, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[229] = idtmkentry(
        __isr_stub_229, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[230] = idtmkentry(
        __isr_stub_230, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[231] = idtmkentry(
        __isr_stub_231, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[232] = idtmkentry(
        __isr_stub_232, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[233] = idtmkentry(
        __isr_stub_233, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[234] = idtmkentry(
        __isr_stub_234, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[235] = idtmkentry(
        __isr_stub_235, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[236] = idtmkentry(
        __isr_stub_236, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[237] = idtmkentry(
        __isr_stub_237, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[238] = idtmkentry(
        __isr_stub_238, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[239] = idtmkentry(
        __isr_stub_239, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[240] = idtmkentry(
        __isr_stub_240, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[241] = idtmkentry(
        __isr_stub_241, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[242] = idtmkentry(
        __isr_stub_242, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[243] = idtmkentry(
        __isr_stub_243, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[244] = idtmkentry(
        __isr_stub_244, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[245] = idtmkentry(
        __isr_stub_245, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[246] = idtmkentry(
        __isr_stub_246, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[247] = idtmkentry(
        __isr_stub_247, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[248] = idtmkentry(
        __isr_stub_248, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[249] = idtmkentry(
        __isr_stub_249, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[250] = idtmkentry(
        __isr_stub_250, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[251] = idtmkentry(
        __isr_stub_251, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[252] = idtmkentry(
        __isr_stub_252, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[253] = idtmkentry(
        __isr_stub_253, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[254] = idtmkentry(
        __isr_stub_254, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[255] = idtmkentry(
        __isr_stub_255, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
#pragma endregion

    idtdescinit(&_idt_desc, _idt, sizeof _idt / sizeof *_idt);
}

void __defexcept(struct exception *regs)
{
    scrputs("--------------- SYSTEM ERROR ---------------\r\n");
    scrputs(
        "The kernel threw a fit trying to handle an interrupt and "
        "couldn't continue.\r\n"); // ah yes.
    scrputsf("Interrupt index: 0x%X\r\n", regs->interrupt_index);
    scrputs("Dump:\r\n");
    scrputsf(
        "    EAX: 0x%X\r\n    EBX: 0x%X\r\n    ECX: 0x%X\r\n    EDX: "
        "0x%X\r\n    ESI: "
        "0x%X\r\n    EDI: "
        "0x%X\r\n    EBP: 0x%X\r\n    ESP: 0x%X\r\n    CS: "
        "0x%X\r\n    DS: "
        "0x%X\r\n    SS: "
        "0x%X\r\n    EFLAGS: 0b%b\r\n",
        regs->accumulator,
        regs->base,
        regs->counter,
        regs->data,
        regs->source,
        regs->destination,
        regs->stack_top_ptr,
        regs->stack_bottom_ptr,
        regs->flags,
        regs->code_segment,
        regs->data_segment,
        regs->stack_segment);

    corehlt();
}
