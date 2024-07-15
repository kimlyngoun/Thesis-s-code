close all; clear;
ec = [0.1783 0.2611 0.4389 0.7000 1.0298 1.4998]';
vn = [0 20 60 120 200 300]';

f = fit(vn, ec, 'poly1');
coeff = coeffvalues(f);
a = coeff(1);
b = coeff(2);
ec_fit = a*vn + b;

plot(vn, ec, 'o');
hold on;
plot(vn, ec_fit);
hold off;
title("EC model for V_w = 35L");
xlabel("V_n [ml]");
ylabel("EC_{35}(V_n) [mS/cm]");
legend("EC_{actual}", "EC_{model}", "Location", "southeast");
str = sprintf("EC_{35}(V_n) = %.4fV_n + %.4f",a,b);
text(10, 1.4, str);