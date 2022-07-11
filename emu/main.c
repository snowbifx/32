#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processor.h"
#include "consts.h"
#include "types.h"
#include "debug.h"
#include "thread.h"

int main() {
    Arch arch;

    initdisk(&arch.processor, "test.bin");
    initmem(&arch.processor, 0xFFFF);

    Drive onload = {.disk = malloc(512), .limit = 512, .signature = DRIVE_SYS | DRIVE_LOAD};
    uint8_t _disk[512] = {
    //    00    01    02    03    04    05    06   07     08    09    0A    0B    0C    0D    0E   0F
        0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 10
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 20
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 30
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 40
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 50
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 60
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 70
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 80
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 90
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // A0
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // B0
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // C0
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // D0
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // E0
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // F0
    };
    memcpy(&arch.processor.memory[0x7E00], _disk, 512);
    memcpy(onload.disk, _disk, 512);
    arch.bios.drives[15] = onload;

    init_thr(&arch);
    thr_join(&arch);

    cleanproc(&arch.processor);

    double seconds = (double)(clock() - arch.processor.start) / CLOCKS_PER_SEC;
    printf(DBGTAG_INFODUMP"finished %I64d instructions in %fs (%f instructions/sec)\n", CALC_TICKSDOT, arch.processor.instructions, seconds, arch.processor.instructions/seconds);

    
    //#ifdef DEBUG
        printf(DBGTAG_INFODUMP"CPUDUMP:\n", CALC_TICKSDOT);
        printf(DBGTAG_INFODUMP"Registers:\n", CALC_TICKSDOT);
        printf(DBGTAG_INFODUMP" | GA=%08x GB=%08x GC=%08x GD=%08x\n"DBGTAG_INFODUMP" | GE=%08x GF=%08x GG=%08x\n",
            CALC_TICKSDOT, arch.processor.registers[ga], arch.processor.registers[gb], arch.processor.registers[gc], arch.processor.registers[gd],
            CALC_TICKSDOT, arch.processor.registers[ge], arch.processor.registers[gf], arch.processor.registers[gg]);
        printf(DBGTAG_INFODUMP" | pv=%d ", CALC_TICKSDOT, (arch.processor.registers[fl] & 0b1100000000000000) >> 14);
        if (testbit(arch.processor.registers[fl], fz)) printf("fz ");
        if (testbit(arch.processor.registers[fl], fc)) printf("fc ");
        if (testbit(arch.processor.registers[fl], fs)) printf("fs ");
        printf("\n");
        printf(DBGTAG_INFODUMP" | PC=%08x, SP=%08x, BP=%08x\n", CALC_TICKSDOT, arch.processor.registers[pc], arch.processor.registers[sp], arch.processor.registers[bp]);
    //#endif

    return 0;
}