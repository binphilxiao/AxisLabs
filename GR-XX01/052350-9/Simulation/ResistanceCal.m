TH_uu=8.2   #Over charge upper threshold 
TH_ud=7.9   #Over charge lower threshold 
TH_du=6.5   #Under charge upper threshold 
TH_dd=6     #Under charge lower threshold 
REF=2       #Reference of comparator
R_u=20      #Upper resistors R1+R2   
A=[REF, REF-TH_uu, REF-TH_uu, 0; REF, REF-TH_ud, REF-TH_ud, REF-TH_ud; REF-TH_du, REF-TH_du, 0, 0; REF-TH_dd, REF-TH_dd, REF-TH_dd, 0]
C=[-2*R_u, -2*R_u, -2*R_u, -2*R_u]
R=inv(A)*transpose(C)
R

