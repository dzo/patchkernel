#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// lookup a symbol name in the kallsysms file
int lookup(char *s) {
	char line[256];
	FILE *fsym=fopen("kallsyms","r");
	int v;

	while(!feof(fsym)) {
		fgets(line,256,fsym);
		if(!strncmp(line+11,s,strlen(s))) {
			line[8]=0;
			v=strtoul(line, NULL, 16)-0xc0008000;
			printf("Found %s at %x\n",s,v);
			return v;
		}
	}
	printf("Didn't find %s\n",s);
	return 0;
}

struct core_speed {
        unsigned int            khz;
        int                     src;
        unsigned int            pri_src_sel;
        unsigned int            sec_src_sel;
        unsigned int            pll_l_val;
};

struct l2_level {
        struct core_speed       speed;
        unsigned int            vdd_dig;
        unsigned int            vdd_mem;
        unsigned int            bw_level;
};

struct acpu_level {
        unsigned int            use_for_scaling;
        struct core_speed       speed;
        int                     l2_level;
        unsigned int            vdd_core;
};

#define PLL_8                    0
#define HFPLL                   -1
#define QSB                     -2

#define STBY_KHZ                1

static struct l2_level l2_freq_tbl_8960_kraitv2[] = {
        [0]  = { {STBY_KHZ, QSB,   0, 0, 0x00 }, 1050000, 1050000, 0 },
        [1]  = { {  384000, PLL_8, 0, 2, 0x00 }, 1050000, 1050000, 1 },
        [2]  = { {  432000, HFPLL, 2, 0, 0x20 }, 1050000, 1050000, 2 },
        [3]  = { {  486000, HFPLL, 2, 0, 0x24 }, 1050000, 1050000, 2 },
        [4]  = { {  540000, HFPLL, 2, 0, 0x28 }, 1050000, 1050000, 2 },
        [5]  = { {  594000, HFPLL, 1, 0, 0x16 }, 1050000, 1050000, 2 },
        [6]  = { {  648000, HFPLL, 1, 0, 0x18 }, 1050000, 1050000, 4 },
        [7]  = { {  702000, HFPLL, 1, 0, 0x1A }, 1050000, 1050000, 4 },
        [8]  = { {  756000, HFPLL, 1, 0, 0x1C }, 1150000, 1150000, 4 },
        [9]  = { {  810000, HFPLL, 1, 0, 0x1E }, 1150000, 1150000, 4 },
        [10] = { {  864000, HFPLL, 1, 0, 0x20 }, 1150000, 1150000, 4 },
        [11] = { {  918000, HFPLL, 1, 0, 0x22 }, 1150000, 1150000, 6 },
        [12] = { {  972000, HFPLL, 1, 0, 0x24 }, 1150000, 1150000, 6 },
        [13] = { { 1026000, HFPLL, 1, 0, 0x26 }, 1150000, 1150000, 6 },
        [14] = { { 1080000, HFPLL, 1, 0, 0x28 }, 1150000, 1150000, 6 },
        [15] = { { 1134000, HFPLL, 1, 0, 0x2A }, 1150000, 1150000, 6 },
        [16] = { { 1188000, HFPLL, 1, 0, 0x2C }, 1150000, 1150000, 6 },
        [17] = { { 1242000, HFPLL, 1, 0, 0x2E }, 1150000, 1150000, 6 },
        [18] = { { 1296000, HFPLL, 1, 0, 0x30 }, 1150000, 1150000, 6 },
        [19] = { { 1350000, HFPLL, 1, 0, 0x32 }, 1150000, 1150000, 6 },
};

#define L2(x) x
//(&l2_freq_tbl_8960_kraitv2[(x)])

static struct acpu_level acpu_freq_tbl_8960_kraitv2_slow[] = {
        { 0, { STBY_KHZ, QSB,   0, 0, 0x00 }, L2(0),   950000 },
        { 1, {   384000, PLL_8, 0, 2, 0x00 }, L2(1),   950000 },
        { 0, {   432000, HFPLL, 2, 0, 0x20 }, L2(7),   975000 },
        { 1, {   486000, HFPLL, 2, 0, 0x24 }, L2(7),   975000 },
        { 0, {   540000, HFPLL, 2, 0, 0x28 }, L2(7),  1000000 },
        { 1, {   594000, HFPLL, 1, 0, 0x16 }, L2(7),  1000000 },
        { 0, {   648000, HFPLL, 1, 0, 0x18 }, L2(7),  1025000 },
        { 1, {   702000, HFPLL, 1, 0, 0x1A }, L2(7),  1025000 },
        { 0, {   756000, HFPLL, 1, 0, 0x1C }, L2(7),  1075000 },
        { 1, {   810000, HFPLL, 1, 0, 0x1E }, L2(7),  1075000 },
        { 0, {   864000, HFPLL, 1, 0, 0x20 }, L2(7),  1100000 },
        { 1, {   918000, HFPLL, 1, 0, 0x22 }, L2(7),  1100000 },
        { 0, {   972000, HFPLL, 1, 0, 0x24 }, L2(7),  1125000 },
        { 1, {  1026000, HFPLL, 1, 0, 0x26 }, L2(7),  1125000 },
//        { 0, {  1080000, HFPLL, 1, 0, 0x28 }, L2(16), 1175000 },
        { 1, {  1134000, HFPLL, 1, 0, 0x2A }, L2(16), 1175000 },
//        { 0, {  1188000, HFPLL, 1, 0, 0x2C }, L2(16), 1200000 },
        { 1, {  1242000, HFPLL, 1, 0, 0x2E }, L2(16), 1200000 },
//        { 0, {  1296000, HFPLL, 1, 0, 0x30 }, L2(16), 1225000 },
        { 1, {  1350000, HFPLL, 1, 0, 0x32 }, L2(16), 1225000 },
//        { 0, {  1404000, HFPLL, 1, 0, 0x34 }, L2(16), 1237500 },
        { 1, {  1458000, HFPLL, 1, 0, 0x36 }, L2(16), 1237500 },
        { 1, {  1512000, HFPLL, 1, 0, 0x38 }, L2(16), 1250000 },
        { 1, {  1620000, HFPLL, 1, 0, 0x3c }, L2(16), 1275000 },
	{ 1, {  1728000, HFPLL, 1, 0, 0x40 }, L2(16), 1300000 },
        { 1, {  1809000, HFPLL, 1, 0, 0x43 }, L2(16), 1300000 },
        { 0, { 0 } }
};

static struct acpu_level acpu_freq_tbl_8960_kraitv2_nom[] = {
        { 0, { STBY_KHZ, QSB,   0, 0, 0x00 }, L2(0),   900000 },
        { 1, {   384000, PLL_8, 0, 2, 0x00 }, L2(1),   900000 },
        { 0, {   432000, HFPLL, 2, 0, 0x20 }, L2(7),   925000 },
        { 1, {   486000, HFPLL, 2, 0, 0x24 }, L2(7),   925000 },
        { 0, {   540000, HFPLL, 2, 0, 0x28 }, L2(7),   950000 },
        { 1, {   594000, HFPLL, 1, 0, 0x16 }, L2(7),   950000 },
        { 0, {   648000, HFPLL, 1, 0, 0x18 }, L2(7),   975000 },
        { 1, {   702000, HFPLL, 1, 0, 0x1A }, L2(7),   975000 },
        { 0, {   756000, HFPLL, 1, 0, 0x1C }, L2(7),  1025000 },
        { 1, {   810000, HFPLL, 1, 0, 0x1E }, L2(7),  1025000 },
        { 0, {   864000, HFPLL, 1, 0, 0x20 }, L2(7),  1050000 },
        { 1, {   918000, HFPLL, 1, 0, 0x22 }, L2(7),  1050000 },
        { 0, {   972000, HFPLL, 1, 0, 0x24 }, L2(7),  1075000 },
        { 1, {  1026000, HFPLL, 1, 0, 0x26 }, L2(7),  1075000 },
//        { 0, {  1080000, HFPLL, 1, 0, 0x28 }, L2(16), 1125000 },
        { 1, {  1134000, HFPLL, 1, 0, 0x2A }, L2(16), 1125000 },
//        { 0, {  1188000, HFPLL, 1, 0, 0x2C }, L2(16), 1150000 },
        { 1, {  1242000, HFPLL, 1, 0, 0x2E }, L2(16), 1150000 },
//        { 0, {  1296000, HFPLL, 1, 0, 0x30 }, L2(16), 1175000 },
        { 1, {  1350000, HFPLL, 1, 0, 0x32 }, L2(16), 1175000 },
//        { 0, {  1404000, HFPLL, 1, 0, 0x34 }, L2(16), 1187500 },
        { 1, {  1458000, HFPLL, 1, 0, 0x36 }, L2(16), 1187500 },
        { 1, {  1512000, HFPLL, 1, 0, 0x38 }, L2(16), 1200000 },
	{ 1, {  1620000, HFPLL, 1, 0, 0x3c }, L2(16), 1225000 },
	{ 1, {  1728000, HFPLL, 1, 0, 0x40 }, L2(16), 1250000 },
	{ 1, {  1809000, HFPLL, 1, 0, 0x43 }, L2(16), 1275000 },
        { 0, { 0 } }
};
static struct acpu_level acpu_freq_tbl_8960_kraitv2_fast[] = {
        { 0, { STBY_KHZ, QSB,   0, 0, 0x00 }, L2(0),   850000 },
        { 1, {   384000, PLL_8, 0, 2, 0x00 }, L2(1),   850000 },
        { 0, {   432000, HFPLL, 2, 0, 0x20 }, L2(7),   875000 },
        { 1, {   486000, HFPLL, 2, 0, 0x24 }, L2(7),   875000 },
        { 0, {   540000, HFPLL, 2, 0, 0x28 }, L2(7),   900000 },
        { 1, {   594000, HFPLL, 1, 0, 0x16 }, L2(7),   900000 },
        { 0, {   648000, HFPLL, 1, 0, 0x18 }, L2(7),   925000 },
        { 1, {   702000, HFPLL, 1, 0, 0x1A }, L2(7),   925000 },
        { 0, {   756000, HFPLL, 1, 0, 0x1C }, L2(7),   975000 },
        { 1, {   810000, HFPLL, 1, 0, 0x1E }, L2(7),   975000 },
        { 0, {   864000, HFPLL, 1, 0, 0x20 }, L2(7),  1000000 },
        { 1, {   918000, HFPLL, 1, 0, 0x22 }, L2(7),  1000000 },
        { 0, {   972000, HFPLL, 1, 0, 0x24 }, L2(7),  1025000 },
        { 1, {  1026000, HFPLL, 1, 0, 0x26 }, L2(7),  1025000 },
//        { 0, {  1080000, HFPLL, 1, 0, 0x28 }, L2(16), 1075000 },
        { 1, {  1134000, HFPLL, 1, 0, 0x2A }, L2(16), 1075000 },
//        { 0, {  1188000, HFPLL, 1, 0, 0x2C }, L2(16), 1100000 },
        { 1, {  1242000, HFPLL, 1, 0, 0x2E }, L2(16), 1100000 },
//        { 0, {  1296000, HFPLL, 1, 0, 0x30 }, L2(16), 1125000 },
        { 1, {  1350000, HFPLL, 1, 0, 0x32 }, L2(16), 1125000 },
//        { 0, {  1404000, HFPLL, 1, 0, 0x34 }, L2(16), 1137500 },
        { 1, {  1458000, HFPLL, 1, 0, 0x36 }, L2(16), 1137500 },
        { 1, {  1512000, HFPLL, 1, 0, 0x38 }, L2(16), 1150000 },
        { 1, {  1620000, HFPLL, 1, 0, 0x3c }, L2(16), 1175000 },
        { 1, {  1728000, HFPLL, 1, 0, 0x40 }, L2(16), 1200000 },
        { 1, {  1809000, HFPLL, 1, 0, 0x43 }, L2(16), 1250000 },
//        { 1, {  1890000, HFPLL, 1, 0, 0x46 }, L2(16), 1275000 },
//	{ 1, {  1998000, HFPLL, 1, 0, 0x4a }, L2(16), 1300000 },

        { 0, { 0 } }
};

int l2;

void update_table(struct acpu_level *t) {
	printf("update table %p\n",t);
	while(t->speed.khz) {
		t->l2_level=l2+sizeof(struct l2_level)*t->l2_level;
		t++;
	}
}

int w(char *a, int v) {
	memcpy(a,(char *)&v,4);
}

int main() {

	FILE *fk=fopen("piggy","rb");
	int t1,t2,t3,i,msm_cpufreq_init,cftc;
	char *mem;
	int sz;

	int instr[]={0xe3010240,0xe3400017,0xe5840020,0xe5840020,0xe3a00000,0xe1a00000,0xe1a00000};

	t1=lookup("acpu_freq_tbl_8960_kraitv2_slow");
	t2=lookup("acpu_freq_tbl_8960_kraitv2_nom");
	t3=lookup("acpu_freq_tbl_8960_kraitv2_fast");
	msm_cpufreq_init=lookup("msm_cpufreq_init");
	cftc=lookup("cpufreq_frequency_table_cpuinfo");

	l2=lookup("l2_freq_tbl_8960_kraitv2")+0xc0008000;

	update_table(acpu_freq_tbl_8960_kraitv2_slow);
	update_table(acpu_freq_tbl_8960_kraitv2_nom);
	update_table(acpu_freq_tbl_8960_kraitv2_fast);

	mem=calloc(20000000,1);
	sz=fread(mem,1,20000000,fk);

	fclose(fk);


	memcpy(mem+t1,acpu_freq_tbl_8960_kraitv2_slow,sizeof(acpu_freq_tbl_8960_kraitv2_slow));
	memcpy(mem+t2,acpu_freq_tbl_8960_kraitv2_nom,sizeof(acpu_freq_tbl_8960_kraitv2_nom));
	memcpy(mem+t3,acpu_freq_tbl_8960_kraitv2_fast,sizeof(acpu_freq_tbl_8960_kraitv2_fast));


	fk=fopen("piggy1","wb");

	fwrite(mem,1,sz,fk);
	fclose(fk);

	return 0;

}
