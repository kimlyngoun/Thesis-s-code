% Data for time (seconds)
time = [0 10 20 30 40 50 60 70 80 90 100];

% Data for volume (arbitrary units)
volume = [0 10 25 29 50 65 75 90 100 112 125];

% Fit a linear equation to the data (least squares method)
[p, S] = polyfit(time, volume, 1);  % Fit a line (degree 1)

% Equation in the form of y = mx + b
m = p(1);  % Slope
b = p(2);  % y-intercept

% Generate x values for the fitted line (more points for smoother visualization)
x_fit = linspace(min(time), max(time), 200);

% Calculate y values for the fitted line using the equation
y_fit = m * x_fit + b;

% Plot the data points
figure;
plot(time, volume, 'o', 'MarkerSize', 8);  % 'o' for circle markers, adjust size
hold on;  % To plot on the same figure as data points

% Plot the fitted line
plot(x_fit, y_fit, 'r-', 'LineWidth', 3);  % Red solid line

% Add labels and title
xlabel('Time (s)');
ylabel('Volume of the nutrient (ml)');  % Adjust units based on your actual data
title('Motor 1');

% Add legend
legend('Data Points', 'Fitted Line');

% Add equation text on the plot
equation_text = sprintf('Vn = %.3ft %.3f', m, b);
text(0.05 * (max(time) - min(time)) + min(time), 0.95 * (max(volume) - min(volume)) + min(volume), ...
    equation_text, 'FontSize', 25, 'BackgroundColor', 'white');

% Show the plot
hold off;