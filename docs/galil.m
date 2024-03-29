% Galil figure 1

M = [5,10,25,50,100,250,500,1000,2500];
y_10k = [0.000080,0.000081,0.000080,0.000080,0.000085,0.000079,0.000074,0.000073,0.000061];
y_25k = [0.000200,0.000200,0.000201,0.000201,0.000201,0.000199,0.000195,0.000192,0.000183];
y_100k = [0.000798,0.000794,0.000796,0.000792,0.000795,0.000797,0.000790,0.000785,0.000773];
y_250k = [0.001979,0.001975,0.001967,0.002082,0.002086,0.002145,0.002012,0.001968,0.001955];
y_1M = [0.008067,0.007882,0.007886,0.007885,0.007887,0.007884,0.007877,0.007870,0.007869];

figure;
semilogx(M,log10(y_10k),'-*',"linewidth",2);
hold on;
semilogx(M,log10(y_25k),'-*',"linewidth",2);
semilogx(M,log10(y_100k),'-*',"linewidth",2);
semilogx(M,log10(y_250k),'-*',"linewidth",2);
semilogx(M,log10(y_1M),'-*',"linewidth",2);
hold off;
title ("Galil algorithm for random bytes sequences");
legend({'N = 10k','N = 25k','N = 100k','N = 250k','N = 1M'},'Location','northeast')
xlabel("pattern length") 
ylabel("log_{10}(time) (s)")
