close; clear; clc;

filename = 'model_base_data.csv';
data = table2array(readtable(filename));

t_sec = data(:,1);
ec = data(:,2);

t_scale = t_sec - ones(size(t_sec))*t_sec(1);
t_minute = t_scale/60;

t_minute_2 = t_minute(t_minute>40&t_minute<70);
ec_2 = ec(t_minute>40&t_minute<70);

figure(1);
plot(t_minute, ec);
rectangle('Position',[40 1.42 30 0.3], 'EdgeColor', 'r');
text(40, 1.73, 'EC control in action');
title('Model-based control over 90 minutes');
xlim([t_minute(1) t_minute(end)]);
xticks([0:10:90]);
xlabel('Time [minutes]');
ylabel('EC [mS/cm]');

figure(2);
plot(t_minute_2, ec_2);
y_pos = 1.4;
y_height = 1.75-y_pos;
rectangle('Position',[44 y_pos 6 y_height], 'EdgeColor', 'r', 'FaceColor', [1 0 0], 'FaceAlpha', 0.1);
text(44.2, 1.73, 'EC start dropping');
rectangle('Position',[50 y_pos 6 y_height], 'EdgeColor', 'r', 'FaceColor', [1 1 0], 'FaceAlpha', 0.1);
text(50.2, 1.73, 'Mixing nutrient A');
rectangle('Position',[56 y_pos 6 y_height], 'EdgeColor', 'r', 'FaceColor', [0 1 0], 'FaceAlpha', 0.1);
text(56.2, 1.73, 'Mixing nutrient B');
text(62.2, 1.73, 'EC stabilized');

title('Zoom-in view of EC control');
xlim([t_minute_2(1) t_minute_2(end)]);
xticks([40:5:65]);
xlabel('Time [minutes]');
ylim([1.4 1.75]);
ylabel('EC [mS/cm]');

