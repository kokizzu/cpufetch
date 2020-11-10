#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "uarch.h"
#include "../common/global.h"

// Data not available
#define NA                   -1

typedef uint32_t MICROARCH;

struct uarch {
  MICROARCH uarch;
  char* uarch_str;
  // int32_t process; process depends on SoC
};

enum cpuinfo_uarch {
  UARCH_UNKNOWN,
  // ARM
  UARCH_ARM7,
  UARCH_ARM9,  
  UARCH_ARM11,       // ARM 1136, ARM 1156, ARM 1176, or ARM 11MPCore.
  UARCH_CORTEX_A5,  
  UARCH_CORTEX_A7,  
  UARCH_CORTEX_A8,    
  UARCH_CORTEX_A9,    
  UARCH_CORTEX_A12,    
  UARCH_CORTEX_A15,    
  UARCH_CORTEX_A17,    
  UARCH_CORTEX_A32,    
  UARCH_CORTEX_A35,    
  UARCH_CORTEX_A53,    
  UARCH_CORTEX_A55r0, // ARM Cortex-A55 revision 0 (restricted dual-issue capabilities compared to revision 1+).
  UARCH_CORTEX_A55, 
  UARCH_CORTEX_A57, 
  UARCH_CORTEX_A65, 
  UARCH_CORTEX_A72, 
  UARCH_CORTEX_A73, 
  UARCH_CORTEX_A75, 
  UARCH_CORTEX_A76,
  UARCH_CORTEX_A77,
  UARCH_CORTEX_A78, 
  UARCH_NEOVERSE_N1,
  UARCH_NEOVERSE_E1,
  UARCH_SCORPION,
  UARCH_KRAIT,
  UARCH_KYRO,
  UARCH_FALKOR,
  UARCH_SAPHIRA,
  UARCH_DENVER,
  UARCH_DENVER2,
  UARCH_CARMEL,
  // SAMSUNG
  UARCH_EXYNOS_M1,  // Samsung Exynos M1 (Exynos 8890 big cores)
  UARCH_EXYNOS_M2,  // Samsung Exynos M2 (Exynos 8895 big cores)
  UARCH_EXYNOS_M3,  // Samsung Exynos M3 (Exynos 9810 big cores)
  UARCH_EXYNOS_M4,  // Samsung Exynos M4 (Exynos 9820 big cores)
  UARCH_EXYNOS_M5,  // Samsung Exynos M5 (Exynos 9830 big cores)
  // APPLE
  UARCH_SWIFT,      // Apple A6 and A6X processors.
  UARCH_CYCLONE,    // Apple A7 processor.
  UARCH_TYPHOON,    // Apple A8 and A8X processor
  UARCH_TWISTER,    // Apple A9 and A9X processor.
  UARCH_HURRICANE,  // Apple A10 and A10X processor.
  UARCH_MONSOON,    // Apple A11 processor (big cores).
  UARCH_MISTRAL,    // Apple A11 processor (little cores).
  UARCH_VORTEX,     // Apple A12 processor (big cores).
  UARCH_TEMPEST,    // Apple A12 processor (big cores).
  UARCH_LIGHTNING,  // Apple A13 processor (big cores).
  UARCH_THUNDER,    // Apple A13 processor (little cores).
  // CAVIUM
  UARCH_THUNDERX,   // Cavium ThunderX
  UARCH_THUNDERX2,  //  Cavium ThunderX2 (originally Broadcom Vulkan).
  // MARVELL
  UARCH_PJ4,
  UARCH_BRAHMA_B15,
  UARCH_BRAHMA_B53,
  UARCH_XGENE,        // Applied Micro X-Gene.
  UARCH_TAISHAN_V110  // HiSilicon TaiShan v110 (Huawei Kunpeng 920 series processors).
};


#define UARCH_START if (false) {}
#define CHECK_UARCH(arch, cpu, im_, p_, v_, r_, cpuname, str, uarch, vendor) \
   else if (im_ == im && p_ == p && (v_ == NA || v_ == v) && (r_ == NA || r_ == r)) fill_uarch(arch, cpu, cpuname, str, uarch, vendor);
#define UARCH_END else { printBug("Unknown microarchitecture detected: IM=0x%.8X P=0x%.8X V=0x%.8X R=0x%.8X", im, p, v, r); fill_uarch(arch, cpu, "Unknown", "Unknown", UARCH_UNKNOWN, CPU_VENDOR_UNKNOWN); }
   
void fill_uarch(struct uarch* arch, struct cpuInfo* cpu, char* cpuname, char* str, MICROARCH u, VENDOR vendor) {
  arch->uarch_str = malloc(sizeof(char) * (strlen(str)+1));
  strcpy(arch->uarch_str, str);
  arch->uarch = u;
  cpu->cpu_vendor = vendor;
  cpu->cpu_name = malloc(sizeof(char) * (strlen(cpuname)+1));
  strcpy(cpu->cpu_name, cpuname);
}   

/*
 * Codes are based on pytorch/cpuinfo, more precisely:
 * https://github.com/pytorch/cpuinfo/blob/master/src/arm/uarch.c
 */
struct uarch* get_uarch_from_midr(uint32_t midr, struct cpuInfo* cpu) {
  struct uarch* arch = malloc(sizeof(struct uarch));
  uint32_t im = midr_get_implementer(midr);
  uint32_t p = midr_get_part(midr);
  uint32_t v = midr_get_variant(midr);
  uint32_t r = midr_get_revision(midr);
  
  // ----------------------------------------------------------------------- //
  // IM: Implementer                                                         //
  // P:  Part                                                                //
  // V:  Variant                                                             //
  // R:  Revision                                                            //
  // ----------------------------------------------------------------------- //
  //                     IM   P      V   R                                   //
  UARCH_START  
  CHECK_UARCH(arch, cpu, 'A', 0xC05, NA, NA, "Cortex-A5",       "ARMv7",          UARCH_CORTEX_A5,    CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xC07, NA, NA, "Cortex-A7",       "ARMv7",          UARCH_CORTEX_A7,    CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xC08, NA, NA, "Cortex-A8",       "ARMv7",          UARCH_CORTEX_A8,    CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xC09, NA, NA, "Cortex-A9",       "ARMv7",          UARCH_CORTEX_A9,    CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xC0C, NA, NA, "Cortex-A12",      "ARMv7",          UARCH_CORTEX_A12,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xC0E, NA, NA, "Cortex-A17",      "ARMv7",          UARCH_CORTEX_A17,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xC0D, NA, NA, "Cortex-A12",      "ARMv7",          UARCH_CORTEX_A12,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xC0F, NA, NA, "Cortex-A15",      "ARMv7",          UARCH_CORTEX_A15,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD01, NA, NA, "Cortex-A32",      "ARMv8 AArch32",  UARCH_CORTEX_A32,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD03, NA, NA, "Cortex-A53",      "ARMv8",          UARCH_CORTEX_A53,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD04, NA, NA, "Cortex-A35",      "ARMv8",          UARCH_CORTEX_A35,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD05, NA,  0, "Cortex-A55",      "ARMv8.2",        UARCH_CORTEX_A55r0, CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD05, NA, NA, "Cortex-A55",      "ARMv8.2",        UARCH_CORTEX_A55,   CPU_VENDOR_ARM)  
  CHECK_UARCH(arch, cpu, 'A', 0xD06, NA, NA, "Cortex-A65",      "ARMv8.2",        UARCH_CORTEX_A65,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD07, NA, NA, "Cortex-A57",      "ARMv8",          UARCH_CORTEX_A57,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD08, NA, NA, "Cortex-A72",      "ARMv8",          UARCH_CORTEX_A72,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD09, NA, NA, "Cortex-A73",      "ARMv8",          UARCH_CORTEX_A73,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD0A, NA, NA, "Cortex-A75",      "ARMv8.2",        UARCH_CORTEX_A75,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD0B, NA, NA, "Cortex-A76",      "ARMv8.2",        UARCH_CORTEX_A76,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD0C, NA, NA, "Neoverse N1",     "ARMv8.2",        UARCH_NEOVERSE_N1,  CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD0D, NA, NA, "Cortex-A77",      "ARMv8.2",        UARCH_CORTEX_A77,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD0E, NA, NA, "Cortex-A76",      "ARMv8.2",        UARCH_CORTEX_A76,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD41, NA, NA, "Cortex-A78",      "ARMv8.2",        UARCH_CORTEX_A78,   CPU_VENDOR_ARM)
  CHECK_UARCH(arch, cpu, 'A', 0xD4A, NA, NA, "Neoverse E1",     "ARMv8.2",        UARCH_NEOVERSE_E1,  CPU_VENDOR_ARM)
     
  CHECK_UARCH(arch, cpu, 'B', 0x00F, NA, NA, "Brahma B15",      "?",              UARCH_BRAHMA_B15,   CPU_VENDOR_BROADCOM)
  CHECK_UARCH(arch, cpu, 'B', 0x100, NA, NA, "Brahma B53",      "?",              UARCH_BRAHMA_B53,   CPU_VENDOR_BROADCOM)
  CHECK_UARCH(arch, cpu, 'B', 0x516, NA, NA, "ThunderX2",       "ARMv8.1",        UARCH_THUNDERX2,    CPU_VENDOR_CAVIUM)
  
  CHECK_UARCH(arch, cpu, 'C', 0x0A0, NA, NA, "ThunderX",        "ARMv8",          UARCH_THUNDERX,     CPU_VENDOR_CAVIUM)
  CHECK_UARCH(arch, cpu, 'C', 0x0A1, NA, NA, "ThunderX 88XX",   "ARMv8",          UARCH_THUNDERX,     CPU_VENDOR_CAVIUM)
  CHECK_UARCH(arch, cpu, 'C', 0x0A2, NA, NA, "ThunderX 81XX",   "ARMv8",          UARCH_THUNDERX,     CPU_VENDOR_CAVIUM)
  CHECK_UARCH(arch, cpu, 'C', 0x0A3, NA, NA, "ThunderX 81XX",   "ARMv8",          UARCH_THUNDERX,     CPU_VENDOR_CAVIUM)
  CHECK_UARCH(arch, cpu, 'C', 0x0AF, NA, NA, "ThunderX2 99XX",  "ARMv8.1",        UARCH_THUNDERX2,    CPU_VENDOR_CAVIUM)
  
  CHECK_UARCH(arch, cpu, 'H', 0xD01, NA, NA, "TaiShan v110",    "ARMv8.2-A",      UARCH_TAISHAN_V110, CPU_VENDOR_HUAWUEI) // Kunpeng 920 series
  CHECK_UARCH(arch, cpu, 'H', 0xD40, NA, NA, "Cortex-A76",      "ARMv8.2",        UARCH_CORTEX_A76,   CPU_VENDOR_ARM)     // Kirin 980 Big/Medium cores -> Cortex-A76
  
  CHECK_UARCH(arch, cpu, 'N', 0x000, NA, NA, "Denver",          "ARMv8",          UARCH_DENVER,       CPU_VENDOR_NVIDIA)
  CHECK_UARCH(arch, cpu, 'N', 0x003, NA, NA, "Denver2",         "ARMv8",          UARCH_DENVER2,      CPU_VENDOR_NVIDIA)
  CHECK_UARCH(arch, cpu, 'N', 0x004, NA, NA, "Carmel",          "ARMv8",          UARCH_CARMEL,       CPU_VENDOR_NVIDIA)
  
  CHECK_UARCH(arch, cpu, 'P', 0x000, NA, NA, "Xgene",           "?",              UARCH_XGENE,        CPU_VENDOR_APM)
  
  CHECK_UARCH(arch, cpu, 'Q', 0x00F, NA, NA, "Scorpion",        "?",              UARCH_SCORPION,     CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x02D, NA, NA, "Scorpion",        "?",              UARCH_KRAIT,        CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x04D,  1,  0, "Krait 200",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x04D,  1,  4, "Krait 200",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x04D,  2,  0, "Krait 300",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x06F,  0,  1, "Krait 200",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x06F,  0,  2, "Krait 200",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x06F,  1,  0, "Krait 300",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x06F,  2,  0, "Krait 400",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM) // Snapdragon 800 MSMxxxx
  CHECK_UARCH(arch, cpu, 'Q', 0x06F,  2,  1, "Krait 400",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM) // Snapdragon 801 MSMxxxxPRO
  CHECK_UARCH(arch, cpu, 'Q', 0x06F,  3,  1, "Krait 450",       "ARMv7-A",        UARCH_KRAIT,        CPU_VENDOR_QUALCOMM)
  CHECK_UARCH(arch, cpu, 'Q', 0x201, NA, NA, "Kryo Silver",     "?",              UARCH_KYRO,         CPU_VENDOR_QUALCOMM) // Qualcomm Snapdragon 821: Low-power Kryo "Silver"
  CHECK_UARCH(arch, cpu, 'Q', 0x205, NA, NA, "Kryo Gold",       "?",              UARCH_KYRO,         CPU_VENDOR_QUALCOMM) // Qualcomm Snapdragon 820 & 821: High-performance Kryo "Gold"
  CHECK_UARCH(arch, cpu, 'Q', 0x211, NA, NA, "Kryo Silver",     "?",              UARCH_KYRO,         CPU_VENDOR_QUALCOMM) // Qualcomm Snapdragon 820: Low-power Kryo "Silver"
  CHECK_UARCH(arch, cpu, 'Q', 0x800, 10,  2, "Kryo 260 Gold",   "d",              UARCH_CORTEX_A73,   CPU_VENDOR_ARM)      // High-performance Kryo 260 (r10p2) -> Cortex-A73
  CHECK_UARCH(arch, cpu, 'Q', 0x800, 10,  1, "Kryo 280 Gold",   "d",              UARCH_CORTEX_A73,   CPU_VENDOR_ARM)      // Kryo 280 (r10p1) "Gold" -> Cortex-A73
  CHECK_UARCH(arch, cpu, 'Q', 0x801, 10,  2, "Kryo 260 Silver", "d",              UARCH_CORTEX_A53,   CPU_VENDOR_ARM)      // High-performance Kryo 260 (r10p2) -> Cortex-A73
  CHECK_UARCH(arch, cpu, 'Q', 0x801, 10,  1, "Kryo 280 Silver", "d",              UARCH_CORTEX_A53,   CPU_VENDOR_ARM)      // Kryo 280 (r10p1) "Gold" -> Cortex-A73
  CHECK_UARCH(arch, cpu, 'Q', 0x802, NA, NA, "Kryo 385 Gold",   "d",              UARCH_CORTEX_A75,   CPU_VENDOR_ARM)      // High-performance Kryo 385 "Gold" -> Cortex-A75
  CHECK_UARCH(arch, cpu, 'Q', 0x803, NA, NA, "Kryo 385 Silver", "d",              UARCH_CORTEX_A55r0, CPU_VENDOR_ARM)      // Low-power Kryo 385 "Silver" -> Cortex-A55r0
  CHECK_UARCH(arch, cpu, 'Q', 0x804, NA, NA, "Kryo 485 Gold",   "d",              UARCH_CORTEX_A76,   CPU_VENDOR_ARM)      // High-performance Kryo 485 "Gold" / "Gold Prime" -> Cortex-A76
  CHECK_UARCH(arch, cpu, 'Q', 0x805, NA, NA, "Kryo 485 Silver", "d",              UARCH_CORTEX_A55,   CPU_VENDOR_ARM)      // Low-performance Kryo 485 "Silver" -> Cortex-A55
  CHECK_UARCH(arch, cpu, 'Q', 0xC00, NA, NA, "Falkor",          "ARMv8",          UARCH_FALKOR,       CPU_VENDOR_QUALCOMM)      
  CHECK_UARCH(arch, cpu, 'Q', 0xC01, NA, NA, "Saphira",         "ARM v8.3-A",     UARCH_SAPHIRA,      CPU_VENDOR_QUALCOMM)     
  
  CHECK_UARCH(arch, cpu, 'S', 0x001, 1,  NA, "Exynos M1",       "ARMv8",         UARCH_EXYNOS_M1,    CPU_VENDOR_SAMSUNG)   // Exynos 8890
  CHECK_UARCH(arch, cpu, 'S', 0x001, 4,  NA, "Exynos M2",       "ARMv8",         UARCH_EXYNOS_M2,    CPU_VENDOR_SAMSUNG)   // Exynos 8895
  CHECK_UARCH(arch, cpu, 'S', 0x002, 1,  NA, "Exynos M3",       "ARMv8",         UARCH_EXYNOS_M3,    CPU_VENDOR_SAMSUNG)   // Exynos 9810
  CHECK_UARCH(arch, cpu, 'S', 0x003, 1,  NA, "Exynos M4",       "ARMv8.2",       UARCH_EXYNOS_M4,    CPU_VENDOR_SAMSUNG)   // Exynos 9820
  CHECK_UARCH(arch, cpu, 'S', 0x004, 1,  NA, "Exynos M5",       "ARMv8.2",       UARCH_EXYNOS_M5,    CPU_VENDOR_SAMSUNG)   // Exynos 9820 (this one looks wrong at uarch.c ...)
            
  CHECK_UARCH(arch, cpu, 'V', 0x581, NA, NA, "PJ4",             "ARMv7",         UARCH_PJ4,          CPU_VENDOR_MARVELL)
  CHECK_UARCH(arch, cpu, 'V', 0x584, NA, NA, "PJ4B-MP",         "ARMv7",         UARCH_PJ4,          CPU_VENDOR_MARVELL)
  
  UARCH_END
  
  return arch;
}

char* get_str_uarch(struct cpuInfo* cpu) {
  return cpu->arch->uarch_str;    
}

char* get_str_process(struct cpuInfo* cpu) {
  char* str = malloc(sizeof(char) * (4+2+1));
  sprintf(str, "%s", "Unknown");
  return str;
}

void free_uarch_struct(struct uarch* arch) {    
  free(arch->uarch_str);
  free(arch);
}
