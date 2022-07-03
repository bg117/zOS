/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <core.h>
#include <gdt.h>
#include <hal.h>
#include <idt.h>
#include <interrupt_info.h>
#include <log.h>
#include <pic.h>
#include <video.h>

#include <misc/bit_macros.h>
#include <misc/log_macros.h>

static struct gdt_entry      _gdt[5];
static struct gdt_descriptor _gdt_desc;

static struct idt_entry      _idt[256];
static struct idt_descriptor _idt_desc;

static void _default_exception_handler(struct interrupt_info *);
static void _default_irq_handler(struct interrupt_info *);

static void (*_handlers[256])(struct interrupt_info *) = { _default_exception_handler };

static void _hal_init_gdt(void);
static void _hal_init_isr(void);

void __dispatch_exception(struct interrupt_info *);

// auto-generated
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

void hal_init(uint8_t pic1_offset, uint8_t pic2_offset)
{
    KLOG("initializing the global descriptor table\n");
    _hal_init_gdt();

    KLOG("loading the GDT\n");
    gdt_descriptor_load(&_gdt_desc, 0x08, 0x10);

    KLOG("initializing the Programmable Interrupt Controller\n");
    pic_init(pic1_offset, pic2_offset);

    KLOG("mapping default IRQ handlers\n");
    for (int i = 0; i < 16; i++)
        hal_map_exception_handler(i + pic1_offset, _default_irq_handler);

    KLOG("initializing interrupt service routines\n");
    _hal_init_isr();

    KLOG("loading the IDT\n");
    idt_descriptor_load(&_idt_desc);
}

void hal_use_default_interrupt_handler(void (*handler)(struct interrupt_info *))
{
    for (int i = 0; i < sizeof _handlers / sizeof *_handlers; i++)
        _handlers[i] = handler;
}

void hal_map_exception_handler(uint8_t vector, void (*handler)(struct interrupt_info *))
{
    KLOG("mapping handler located at %p to vector %hhu\n", handler, vector);
    _handlers[vector] = handler;
}

void _hal_init_gdt()
{
    _gdt[0] = gdt_make_entry(0x0000, 0x0000, 0x0000, 0x0000);
    _gdt[1]
        = gdt_make_entry(0xFFFFF,
                         0x0000,
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_CODE | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                         GDT_OF_GRANULARITY | GDT_OF_32BIT);
    _gdt[2] = gdt_make_entry(0xFFFFF,
                             0x0000,
                             GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_DATA | GDT_AX_DATA_WRITABLE,
                             GDT_OF_GRANULARITY | GDT_OF_32BIT);
    _gdt[3]
        = gdt_make_entry(0xFFFFF,
                         0x0000,
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING3 | GDT_AX_CODE | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                         GDT_OF_GRANULARITY | GDT_OF_32BIT);
    _gdt[4] = gdt_make_entry(0xFFFFF,
                             0x0000,
                             GDT_AX_PRESENT | GDT_AX_PRIV_RING3 | GDT_AX_DATA | GDT_AX_DATA_WRITABLE,
                             GDT_OF_GRANULARITY | GDT_OF_32BIT);

    gdt_descriptor_init(&_gdt_desc, _gdt, sizeof _gdt / sizeof *_gdt);
}

void _hal_init_isr()
{
    // oh, and, before you ask, this part was generated by a shell for-loop
    // ;)
#pragma region ISR initialization
    _idt[0]   = idt_make_entry(__isr_stub_0, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[1]   = idt_make_entry(__isr_stub_1, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[2]   = idt_make_entry(__isr_stub_2, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[3]   = idt_make_entry(__isr_stub_3, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[4]   = idt_make_entry(__isr_stub_4, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[5]   = idt_make_entry(__isr_stub_5, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[6]   = idt_make_entry(__isr_stub_6, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[7]   = idt_make_entry(__isr_stub_7, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[8]   = idt_make_entry(__isr_stub_8, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[9]   = idt_make_entry(__isr_stub_9, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[10]  = idt_make_entry(__isr_stub_10, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[11]  = idt_make_entry(__isr_stub_11, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[12]  = idt_make_entry(__isr_stub_12, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[13]  = idt_make_entry(__isr_stub_13, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[14]  = idt_make_entry(__isr_stub_14, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[15]  = idt_make_entry(__isr_stub_15, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[16]  = idt_make_entry(__isr_stub_16, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[17]  = idt_make_entry(__isr_stub_17, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[18]  = idt_make_entry(__isr_stub_18, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[19]  = idt_make_entry(__isr_stub_19, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[20]  = idt_make_entry(__isr_stub_20, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[21]  = idt_make_entry(__isr_stub_21, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[22]  = idt_make_entry(__isr_stub_22, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[23]  = idt_make_entry(__isr_stub_23, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[24]  = idt_make_entry(__isr_stub_24, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[25]  = idt_make_entry(__isr_stub_25, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[26]  = idt_make_entry(__isr_stub_26, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[27]  = idt_make_entry(__isr_stub_27, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[28]  = idt_make_entry(__isr_stub_28, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[29]  = idt_make_entry(__isr_stub_29, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[30]  = idt_make_entry(__isr_stub_30, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[31]  = idt_make_entry(__isr_stub_31, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[32]  = idt_make_entry(__isr_stub_32, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[33]  = idt_make_entry(__isr_stub_33, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[34]  = idt_make_entry(__isr_stub_34, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[35]  = idt_make_entry(__isr_stub_35, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[36]  = idt_make_entry(__isr_stub_36, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[37]  = idt_make_entry(__isr_stub_37, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[38]  = idt_make_entry(__isr_stub_38, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[39]  = idt_make_entry(__isr_stub_39, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[40]  = idt_make_entry(__isr_stub_40, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[41]  = idt_make_entry(__isr_stub_41, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[42]  = idt_make_entry(__isr_stub_42, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[43]  = idt_make_entry(__isr_stub_43, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[44]  = idt_make_entry(__isr_stub_44, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[45]  = idt_make_entry(__isr_stub_45, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[46]  = idt_make_entry(__isr_stub_46, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[47]  = idt_make_entry(__isr_stub_47, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[48]  = idt_make_entry(__isr_stub_48, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[49]  = idt_make_entry(__isr_stub_49, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[50]  = idt_make_entry(__isr_stub_50, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[51]  = idt_make_entry(__isr_stub_51, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[52]  = idt_make_entry(__isr_stub_52, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[53]  = idt_make_entry(__isr_stub_53, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[54]  = idt_make_entry(__isr_stub_54, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[55]  = idt_make_entry(__isr_stub_55, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[56]  = idt_make_entry(__isr_stub_56, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[57]  = idt_make_entry(__isr_stub_57, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[58]  = idt_make_entry(__isr_stub_58, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[59]  = idt_make_entry(__isr_stub_59, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[60]  = idt_make_entry(__isr_stub_60, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[61]  = idt_make_entry(__isr_stub_61, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[62]  = idt_make_entry(__isr_stub_62, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[63]  = idt_make_entry(__isr_stub_63, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[64]  = idt_make_entry(__isr_stub_64, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[65]  = idt_make_entry(__isr_stub_65, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[66]  = idt_make_entry(__isr_stub_66, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[67]  = idt_make_entry(__isr_stub_67, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[68]  = idt_make_entry(__isr_stub_68, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[69]  = idt_make_entry(__isr_stub_69, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[70]  = idt_make_entry(__isr_stub_70, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[71]  = idt_make_entry(__isr_stub_71, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[72]  = idt_make_entry(__isr_stub_72, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[73]  = idt_make_entry(__isr_stub_73, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[74]  = idt_make_entry(__isr_stub_74, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[75]  = idt_make_entry(__isr_stub_75, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[76]  = idt_make_entry(__isr_stub_76, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[77]  = idt_make_entry(__isr_stub_77, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[78]  = idt_make_entry(__isr_stub_78, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[79]  = idt_make_entry(__isr_stub_79, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[80]  = idt_make_entry(__isr_stub_80, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[81]  = idt_make_entry(__isr_stub_81, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[82]  = idt_make_entry(__isr_stub_82, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[83]  = idt_make_entry(__isr_stub_83, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[84]  = idt_make_entry(__isr_stub_84, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[85]  = idt_make_entry(__isr_stub_85, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[86]  = idt_make_entry(__isr_stub_86, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[87]  = idt_make_entry(__isr_stub_87, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[88]  = idt_make_entry(__isr_stub_88, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[89]  = idt_make_entry(__isr_stub_89, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[90]  = idt_make_entry(__isr_stub_90, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[91]  = idt_make_entry(__isr_stub_91, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[92]  = idt_make_entry(__isr_stub_92, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[93]  = idt_make_entry(__isr_stub_93, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[94]  = idt_make_entry(__isr_stub_94, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[95]  = idt_make_entry(__isr_stub_95, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[96]  = idt_make_entry(__isr_stub_96, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[97]  = idt_make_entry(__isr_stub_97, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[98]  = idt_make_entry(__isr_stub_98, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[99]  = idt_make_entry(__isr_stub_99, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[100] = idt_make_entry(__isr_stub_100, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[101] = idt_make_entry(__isr_stub_101, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[102] = idt_make_entry(__isr_stub_102, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[103] = idt_make_entry(__isr_stub_103, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[104] = idt_make_entry(__isr_stub_104, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[105] = idt_make_entry(__isr_stub_105, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[106] = idt_make_entry(__isr_stub_106, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[107] = idt_make_entry(__isr_stub_107, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[108] = idt_make_entry(__isr_stub_108, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[109] = idt_make_entry(__isr_stub_109, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[110] = idt_make_entry(__isr_stub_110, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[111] = idt_make_entry(__isr_stub_111, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[112] = idt_make_entry(__isr_stub_112, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[113] = idt_make_entry(__isr_stub_113, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[114] = idt_make_entry(__isr_stub_114, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[115] = idt_make_entry(__isr_stub_115, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[116] = idt_make_entry(__isr_stub_116, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[117] = idt_make_entry(__isr_stub_117, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[118] = idt_make_entry(__isr_stub_118, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[119] = idt_make_entry(__isr_stub_119, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[120] = idt_make_entry(__isr_stub_120, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[121] = idt_make_entry(__isr_stub_121, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[122] = idt_make_entry(__isr_stub_122, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[123] = idt_make_entry(__isr_stub_123, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[124] = idt_make_entry(__isr_stub_124, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[125] = idt_make_entry(__isr_stub_125, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[126] = idt_make_entry(__isr_stub_126, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[127] = idt_make_entry(__isr_stub_127, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[128] = idt_make_entry(__isr_stub_128, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[129] = idt_make_entry(__isr_stub_129, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[130] = idt_make_entry(__isr_stub_130, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[131] = idt_make_entry(__isr_stub_131, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[132] = idt_make_entry(__isr_stub_132, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[133] = idt_make_entry(__isr_stub_133, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[134] = idt_make_entry(__isr_stub_134, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[135] = idt_make_entry(__isr_stub_135, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[136] = idt_make_entry(__isr_stub_136, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[137] = idt_make_entry(__isr_stub_137, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[138] = idt_make_entry(__isr_stub_138, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[139] = idt_make_entry(__isr_stub_139, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[140] = idt_make_entry(__isr_stub_140, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[141] = idt_make_entry(__isr_stub_141, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[142] = idt_make_entry(__isr_stub_142, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[143] = idt_make_entry(__isr_stub_143, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[144] = idt_make_entry(__isr_stub_144, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[145] = idt_make_entry(__isr_stub_145, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[146] = idt_make_entry(__isr_stub_146, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[147] = idt_make_entry(__isr_stub_147, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[148] = idt_make_entry(__isr_stub_148, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[149] = idt_make_entry(__isr_stub_149, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[150] = idt_make_entry(__isr_stub_150, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[151] = idt_make_entry(__isr_stub_151, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[152] = idt_make_entry(__isr_stub_152, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[153] = idt_make_entry(__isr_stub_153, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[154] = idt_make_entry(__isr_stub_154, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[155] = idt_make_entry(__isr_stub_155, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[156] = idt_make_entry(__isr_stub_156, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[157] = idt_make_entry(__isr_stub_157, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[158] = idt_make_entry(__isr_stub_158, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[159] = idt_make_entry(__isr_stub_159, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[160] = idt_make_entry(__isr_stub_160, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[161] = idt_make_entry(__isr_stub_161, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[162] = idt_make_entry(__isr_stub_162, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[163] = idt_make_entry(__isr_stub_163, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[164] = idt_make_entry(__isr_stub_164, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[165] = idt_make_entry(__isr_stub_165, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[166] = idt_make_entry(__isr_stub_166, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[167] = idt_make_entry(__isr_stub_167, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[168] = idt_make_entry(__isr_stub_168, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[169] = idt_make_entry(__isr_stub_169, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[170] = idt_make_entry(__isr_stub_170, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[171] = idt_make_entry(__isr_stub_171, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[172] = idt_make_entry(__isr_stub_172, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[173] = idt_make_entry(__isr_stub_173, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[174] = idt_make_entry(__isr_stub_174, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[175] = idt_make_entry(__isr_stub_175, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[176] = idt_make_entry(__isr_stub_176, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[177] = idt_make_entry(__isr_stub_177, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[178] = idt_make_entry(__isr_stub_178, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[179] = idt_make_entry(__isr_stub_179, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[180] = idt_make_entry(__isr_stub_180, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[181] = idt_make_entry(__isr_stub_181, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[182] = idt_make_entry(__isr_stub_182, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[183] = idt_make_entry(__isr_stub_183, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[184] = idt_make_entry(__isr_stub_184, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[185] = idt_make_entry(__isr_stub_185, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[186] = idt_make_entry(__isr_stub_186, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[187] = idt_make_entry(__isr_stub_187, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[188] = idt_make_entry(__isr_stub_188, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[189] = idt_make_entry(__isr_stub_189, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[190] = idt_make_entry(__isr_stub_190, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[191] = idt_make_entry(__isr_stub_191, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[192] = idt_make_entry(__isr_stub_192, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[193] = idt_make_entry(__isr_stub_193, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[194] = idt_make_entry(__isr_stub_194, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[195] = idt_make_entry(__isr_stub_195, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[196] = idt_make_entry(__isr_stub_196, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[197] = idt_make_entry(__isr_stub_197, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[198] = idt_make_entry(__isr_stub_198, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[199] = idt_make_entry(__isr_stub_199, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[200] = idt_make_entry(__isr_stub_200, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[201] = idt_make_entry(__isr_stub_201, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[202] = idt_make_entry(__isr_stub_202, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[203] = idt_make_entry(__isr_stub_203, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[204] = idt_make_entry(__isr_stub_204, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[205] = idt_make_entry(__isr_stub_205, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[206] = idt_make_entry(__isr_stub_206, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[207] = idt_make_entry(__isr_stub_207, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[208] = idt_make_entry(__isr_stub_208, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[209] = idt_make_entry(__isr_stub_209, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[210] = idt_make_entry(__isr_stub_210, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[211] = idt_make_entry(__isr_stub_211, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[212] = idt_make_entry(__isr_stub_212, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[213] = idt_make_entry(__isr_stub_213, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[214] = idt_make_entry(__isr_stub_214, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[215] = idt_make_entry(__isr_stub_215, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[216] = idt_make_entry(__isr_stub_216, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[217] = idt_make_entry(__isr_stub_217, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[218] = idt_make_entry(__isr_stub_218, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[219] = idt_make_entry(__isr_stub_219, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[220] = idt_make_entry(__isr_stub_220, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[221] = idt_make_entry(__isr_stub_221, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[222] = idt_make_entry(__isr_stub_222, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[223] = idt_make_entry(__isr_stub_223, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[224] = idt_make_entry(__isr_stub_224, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[225] = idt_make_entry(__isr_stub_225, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[226] = idt_make_entry(__isr_stub_226, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[227] = idt_make_entry(__isr_stub_227, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[228] = idt_make_entry(__isr_stub_228, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[229] = idt_make_entry(__isr_stub_229, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[230] = idt_make_entry(__isr_stub_230, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[231] = idt_make_entry(__isr_stub_231, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[232] = idt_make_entry(__isr_stub_232, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[233] = idt_make_entry(__isr_stub_233, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[234] = idt_make_entry(__isr_stub_234, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[235] = idt_make_entry(__isr_stub_235, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[236] = idt_make_entry(__isr_stub_236, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[237] = idt_make_entry(__isr_stub_237, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[238] = idt_make_entry(__isr_stub_238, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[239] = idt_make_entry(__isr_stub_239, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[240] = idt_make_entry(__isr_stub_240, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[241] = idt_make_entry(__isr_stub_241, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[242] = idt_make_entry(__isr_stub_242, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[243] = idt_make_entry(__isr_stub_243, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[244] = idt_make_entry(__isr_stub_244, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[245] = idt_make_entry(__isr_stub_245, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[246] = idt_make_entry(__isr_stub_246, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[247] = idt_make_entry(__isr_stub_247, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[248] = idt_make_entry(__isr_stub_248, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[249] = idt_make_entry(__isr_stub_249, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[250] = idt_make_entry(__isr_stub_250, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[251] = idt_make_entry(__isr_stub_251, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[252] = idt_make_entry(__isr_stub_252, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[253] = idt_make_entry(__isr_stub_253, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[254] = idt_make_entry(__isr_stub_254, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
    _idt[255] = idt_make_entry(__isr_stub_255, 0x08, IDT_AX_RING0 | IDT_AX_I386_INT | IDT_AX_PRESENT);
#pragma endregion

    idt_descriptor_init(&_idt_desc, _idt, sizeof _idt / sizeof *_idt);
}

void __dispatch_exception(struct interrupt_info *info)
{
    _handlers[info->vector](info);
}

void _default_exception_handler(struct interrupt_info *info)
{
}

void _default_irq_handler(struct interrupt_info *info)
{
    pic_send_eoi(info->vector - pic_get_pic1_offset());
}
