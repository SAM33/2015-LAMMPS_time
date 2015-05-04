# LAMMPS_time
使用方法:<br/>
編譯<br/>
g++ lammps_foretell.cpp<br/>
執行LAMMPS並產生LOG檔案<br/>
mpirun -np 4 lmp_mpi < in.crack | tee log.txt<br/>
執行到一半把程式中止<br/>
ctrl^c<br/>
使用估計時間程式<br/>
./a.out [預計模擬的Step數] < log.txt<br/>
程式會算出需要多少時間來執行<br/>
<br/>
注意:<br/>
LAMMPS input 檔案 EX: crack.in必須新增此行<br/>
thermo_style    custom step temp epair emol etotal press cpu<br/>
因為本程式是使用cpu,step欄位來作線性估計<br/>
