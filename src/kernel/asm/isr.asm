; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

bits 32

;**************************
;                         *
;   x86 helper routines   *
;                         *
;**************************
;
; Filename: isr.asm
; Description: Interrupt service routines
;
; Date created: 280622DDMMYY1721HHmm
;
;

extern __dispatch_exception

%macro isr_errcode_stub 1
global __isr_stub_%+%1
__isr_stub_%+%1:    push    %1
                    jmp     _isr_ind
%endmacro

%macro isr_noerrcode_stub 1
global __isr_stub_%+%1
__isr_stub_%+%1:    push    0
                    push    %1
                    jmp     _isr_ind
%endmacro

isr_noerrcode_stub 0
isr_noerrcode_stub 1
isr_noerrcode_stub 2
isr_noerrcode_stub 3
isr_noerrcode_stub 4
isr_noerrcode_stub 5
isr_noerrcode_stub 6
isr_noerrcode_stub 7
isr_errcode_stub   8
isr_noerrcode_stub 9
isr_errcode_stub   10
isr_errcode_stub   11
isr_errcode_stub   12
isr_errcode_stub   13
isr_errcode_stub   14
isr_noerrcode_stub 15
isr_noerrcode_stub 16
isr_errcode_stub   17
isr_noerrcode_stub 18
isr_noerrcode_stub 19
isr_noerrcode_stub 20
isr_noerrcode_stub 21
isr_noerrcode_stub 22
isr_noerrcode_stub 23
isr_noerrcode_stub 24
isr_noerrcode_stub 25
isr_noerrcode_stub 26
isr_noerrcode_stub 27
isr_noerrcode_stub 28
isr_noerrcode_stub 29
isr_errcode_stub   30
isr_noerrcode_stub 31
isr_noerrcode_stub 32
isr_noerrcode_stub 33
isr_noerrcode_stub 34
isr_noerrcode_stub 35
isr_noerrcode_stub 36
isr_noerrcode_stub 37
isr_noerrcode_stub 38
isr_noerrcode_stub 39
isr_noerrcode_stub 40
isr_noerrcode_stub 41
isr_noerrcode_stub 42
isr_noerrcode_stub 43
isr_noerrcode_stub 44
isr_noerrcode_stub 45
isr_noerrcode_stub 46
isr_noerrcode_stub 47
isr_noerrcode_stub 48
isr_noerrcode_stub 49
isr_noerrcode_stub 50
isr_noerrcode_stub 51
isr_noerrcode_stub 52
isr_noerrcode_stub 53
isr_noerrcode_stub 54
isr_noerrcode_stub 55
isr_noerrcode_stub 56
isr_noerrcode_stub 57
isr_noerrcode_stub 58
isr_noerrcode_stub 59
isr_noerrcode_stub 60
isr_noerrcode_stub 61
isr_noerrcode_stub 62
isr_noerrcode_stub 63
isr_noerrcode_stub 64
isr_noerrcode_stub 65
isr_noerrcode_stub 66
isr_noerrcode_stub 67
isr_noerrcode_stub 68
isr_noerrcode_stub 69
isr_noerrcode_stub 70
isr_noerrcode_stub 71
isr_noerrcode_stub 72
isr_noerrcode_stub 73
isr_noerrcode_stub 74
isr_noerrcode_stub 75
isr_noerrcode_stub 76
isr_noerrcode_stub 77
isr_noerrcode_stub 78
isr_noerrcode_stub 79
isr_noerrcode_stub 80
isr_noerrcode_stub 81
isr_noerrcode_stub 82
isr_noerrcode_stub 83
isr_noerrcode_stub 84
isr_noerrcode_stub 85
isr_noerrcode_stub 86
isr_noerrcode_stub 87
isr_noerrcode_stub 88
isr_noerrcode_stub 89
isr_noerrcode_stub 90
isr_noerrcode_stub 91
isr_noerrcode_stub 92
isr_noerrcode_stub 93
isr_noerrcode_stub 94
isr_noerrcode_stub 95
isr_noerrcode_stub 96
isr_noerrcode_stub 97
isr_noerrcode_stub 98
isr_noerrcode_stub 99
isr_noerrcode_stub 100
isr_noerrcode_stub 101
isr_noerrcode_stub 102
isr_noerrcode_stub 103
isr_noerrcode_stub 104
isr_noerrcode_stub 105
isr_noerrcode_stub 106
isr_noerrcode_stub 107
isr_noerrcode_stub 108
isr_noerrcode_stub 109
isr_noerrcode_stub 110
isr_noerrcode_stub 111
isr_noerrcode_stub 112
isr_noerrcode_stub 113
isr_noerrcode_stub 114
isr_noerrcode_stub 115
isr_noerrcode_stub 116
isr_noerrcode_stub 117
isr_noerrcode_stub 118
isr_noerrcode_stub 119
isr_noerrcode_stub 120
isr_noerrcode_stub 121
isr_noerrcode_stub 122
isr_noerrcode_stub 123
isr_noerrcode_stub 124
isr_noerrcode_stub 125
isr_noerrcode_stub 126
isr_noerrcode_stub 127
isr_noerrcode_stub 128
isr_noerrcode_stub 129
isr_noerrcode_stub 130
isr_noerrcode_stub 131
isr_noerrcode_stub 132
isr_noerrcode_stub 133
isr_noerrcode_stub 134
isr_noerrcode_stub 135
isr_noerrcode_stub 136
isr_noerrcode_stub 137
isr_noerrcode_stub 138
isr_noerrcode_stub 139
isr_noerrcode_stub 140
isr_noerrcode_stub 141
isr_noerrcode_stub 142
isr_noerrcode_stub 143
isr_noerrcode_stub 144
isr_noerrcode_stub 145
isr_noerrcode_stub 146
isr_noerrcode_stub 147
isr_noerrcode_stub 148
isr_noerrcode_stub 149
isr_noerrcode_stub 150
isr_noerrcode_stub 151
isr_noerrcode_stub 152
isr_noerrcode_stub 153
isr_noerrcode_stub 154
isr_noerrcode_stub 155
isr_noerrcode_stub 156
isr_noerrcode_stub 157
isr_noerrcode_stub 158
isr_noerrcode_stub 159
isr_noerrcode_stub 160
isr_noerrcode_stub 161
isr_noerrcode_stub 162
isr_noerrcode_stub 163
isr_noerrcode_stub 164
isr_noerrcode_stub 165
isr_noerrcode_stub 166
isr_noerrcode_stub 167
isr_noerrcode_stub 168
isr_noerrcode_stub 169
isr_noerrcode_stub 170
isr_noerrcode_stub 171
isr_noerrcode_stub 172
isr_noerrcode_stub 173
isr_noerrcode_stub 174
isr_noerrcode_stub 175
isr_noerrcode_stub 176
isr_noerrcode_stub 177
isr_noerrcode_stub 178
isr_noerrcode_stub 179
isr_noerrcode_stub 180
isr_noerrcode_stub 181
isr_noerrcode_stub 182
isr_noerrcode_stub 183
isr_noerrcode_stub 184
isr_noerrcode_stub 185
isr_noerrcode_stub 186
isr_noerrcode_stub 187
isr_noerrcode_stub 188
isr_noerrcode_stub 189
isr_noerrcode_stub 190
isr_noerrcode_stub 191
isr_noerrcode_stub 192
isr_noerrcode_stub 193
isr_noerrcode_stub 194
isr_noerrcode_stub 195
isr_noerrcode_stub 196
isr_noerrcode_stub 197
isr_noerrcode_stub 198
isr_noerrcode_stub 199
isr_noerrcode_stub 200
isr_noerrcode_stub 201
isr_noerrcode_stub 202
isr_noerrcode_stub 203
isr_noerrcode_stub 204
isr_noerrcode_stub 205
isr_noerrcode_stub 206
isr_noerrcode_stub 207
isr_noerrcode_stub 208
isr_noerrcode_stub 209
isr_noerrcode_stub 210
isr_noerrcode_stub 211
isr_noerrcode_stub 212
isr_noerrcode_stub 213
isr_noerrcode_stub 214
isr_noerrcode_stub 215
isr_noerrcode_stub 216
isr_noerrcode_stub 217
isr_noerrcode_stub 218
isr_noerrcode_stub 219
isr_noerrcode_stub 220
isr_noerrcode_stub 221
isr_noerrcode_stub 222
isr_noerrcode_stub 223
isr_noerrcode_stub 224
isr_noerrcode_stub 225
isr_noerrcode_stub 226
isr_noerrcode_stub 227
isr_noerrcode_stub 228
isr_noerrcode_stub 229
isr_noerrcode_stub 230
isr_noerrcode_stub 231
isr_noerrcode_stub 232
isr_noerrcode_stub 233
isr_noerrcode_stub 234
isr_noerrcode_stub 235
isr_noerrcode_stub 236
isr_noerrcode_stub 237
isr_noerrcode_stub 238
isr_noerrcode_stub 239
isr_noerrcode_stub 240
isr_noerrcode_stub 241
isr_noerrcode_stub 242
isr_noerrcode_stub 243
isr_noerrcode_stub 244
isr_noerrcode_stub 245
isr_noerrcode_stub 246
isr_noerrcode_stub 247
isr_noerrcode_stub 248
isr_noerrcode_stub 249
isr_noerrcode_stub 250
isr_noerrcode_stub 251
isr_noerrcode_stub 252
isr_noerrcode_stub 253
isr_noerrcode_stub 254
isr_noerrcode_stub 255

_isr_ind:   pushad
            cld

            xor     eax, eax
            mov     ax, ds
            push    eax

            mov     ax, 0x10
            mov     ds, ax
            mov     es, ax
            mov     fs, ax
            mov     gs, ax

            push    esp ; pass exception_info struct
            call    __dispatch_exception
            add     esp, 4

            pop     eax
            mov     ds, ax
            mov     es, ax
            mov     fs, ax
            mov     gs, ax

            popad

            add     esp, 8
            iret
