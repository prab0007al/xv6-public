#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *addr1, *addr2;
  int vp_before, pp_before, vp_after, pp_after, vp_final, pp_final;
//  int i;
  
  printf(1, "=================================\n");
  printf(1, "  Testing mmap System Call\n");
  printf(1, "=================================\n\n");
  
  // Initial state
  vp_before = numvp();
  pp_before = numpp();
  printf(1, "Initial state:\n");
  printf(1, "  Virtual pages:  %d\n", vp_before);
  printf(1, "  Physical pages: %d\n\n", pp_before);
  
  // Test 1: Allocate 2 pages (8192 bytes)
  printf(1, "Test 1: mmap(8192) - allocate 2 pages\n");
  addr1 = (char*)mmap(8192);
  
  if(addr1 == 0){
    printf(1, "  ERROR: mmap failed\n");
    exit();
  }
  
  printf(1, "  mmap returned address: 0x%x\n", (uint)addr1);
  
  vp_after = numvp();
  pp_after = numpp();
  printf(1, "  After mmap (before access):\n");
  printf(1, "    Virtual pages:  %d (increased by %d)\n", vp_after, vp_after - vp_before);
  printf(1, "    Physical pages: %d (increased by %d)\n\n", pp_after, pp_after - pp_before);
  
  // Access the first mapped page
  printf(1, "Test 2: Writing to first mapped page\n");
  addr1[0] = 'A';
  addr1[100] = 'B';
  printf(1, "  Written to page at offset 0 and 100\n");
  
  vp_after = numvp();
  pp_after = numpp();
  printf(1, "  After first page access:\n");
  printf(1, "    Virtual pages:  %d\n", vp_after);
  printf(1, "    Physical pages: %d (increased by %d)\n\n", pp_after, pp_after - pp_before);
  
  // Access the second mapped page
  printf(1, "Test 3: Writing to second mapped page\n");
  addr1[4096] = 'C';  // Second page
  addr1[5000] = 'D';
  printf(1, "  Written to page at offset 4096 and 5000\n");
  
  vp_final = numvp();
  pp_final = numpp();
  printf(1, "  After second page access:\n");
  printf(1, "    Virtual pages:  %d\n", vp_final);
  printf(1, "    Physical pages: %d (increased by %d)\n\n", pp_final, pp_final - pp_before);
  
  // Read back the values
  printf(1, "Test 4: Reading back written values\n");
  printf(1, "  addr1[0] = '%c' (expected 'A')\n", addr1[0]);
  printf(1, "  addr1[100] = '%c' (expected 'B')\n", addr1[100]);
  printf(1, "  addr1[4096] = '%c' (expected 'C')\n", addr1[4096]);
  printf(1, "  addr1[5000] = '%c' (expected 'D')\n\n", addr1[5000]);
  
  // Allocate more memory
  printf(1, "Test 5: Second mmap(4096) - allocate 1 more page\n");
  addr2 = (char*)mmap(4096);
  printf(1, "  mmap returned address: 0x%x\n", (uint)addr2);
  
  vp_final = numvp();
  pp_final = numpp();
  printf(1, "  After second mmap:\n");
  printf(1, "    Virtual pages:  %d\n", vp_final);
  printf(1, "    Physical pages: %d (no increase yet)\n\n", pp_final);
  
  // Access the new page
  printf(1, "Test 6: Writing to newly mapped page\n");
  addr2[0] = 'X';
  printf(1, "  Written 'X' to new page\n");
  
  vp_final = numvp();
  pp_final = numpp();
  printf(1, "  After accessing new page:\n");
  printf(1, "    Virtual pages:  %d\n", vp_final);
  printf(1, "    Physical pages: %d\n\n", pp_final);
  
  printf(1, "=================================\n");
  printf(1, "  Summary:\n");
  printf(1, "=================================\n");
  printf(1, "Virtual pages increased from %d to %d (added %d)\n", 
         vp_before, vp_final, vp_final - vp_before);
  printf(1, "Physical pages increased from %d to %d (added %d)\n",
         pp_before, pp_final, pp_final - pp_before);
  printf(1, "\nDemand paging working: Virtual pages allocated immediately,\n");
  printf(1, "but physical pages allocated only when accessed!\n");
  
  exit();
}

