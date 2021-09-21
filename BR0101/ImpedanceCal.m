w=12.5  # Trace width
s=6       # Trace separation
t=1.2     # Trace thickness
h=10   # Dielectric thickness
er=4.2    # Dielectric constant
z0=87*log(5.98*h/(0.8*w+t))/(1.41+er)^0.5
zd=2*z0*(1-0.48*exp(-1*0.96*s/h))