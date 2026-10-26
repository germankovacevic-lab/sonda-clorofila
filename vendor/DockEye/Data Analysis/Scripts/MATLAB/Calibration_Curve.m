close all
clear all

% Load data
data = readtable('8-14-25 chla cal.xlsx');

y = data(:,1);
xd = data(:,2);
xh = data(:,3);

% convert to array

xd2 = table2array(xd);
xh2 = table2array(xh);
y2 = table2array(y);

% Linear regression
regD = fitlm(xd2,y2);
regH = fitlm(xh2,y2);

% Get coefficients
coeffs = regD.Coefficients.Estimate;
slope = coeffs(2);
intercept = coeffs(1);
r2 = regD.Rsquared.Ordinary;

coeffs2 = regH.Coefficients.Estimate;
slope2 = coeffs2(2);
intercept2 = coeffs2(1);
r22 = regH.Rsquared.Ordinary;

% Create fit line
xdfit = linspace(min(xd2), max(xd2), 100);
ydfit = slope * xdfit + intercept;

xhfit = linspace(min(xh2), max(xh2), 100);
yhfit = slope2 * xhfit + intercept2;

% plots

subplot(2,1,1)
plot(xd2, y2, 'wo', 'MarkerSize', 6); hold on;
plot(xdfit, ydfit, 'b-', 'LineWidth', 2, 'DisplayName', 'Fit');
xlabel('Measured counts', 'FontSize', 20);
ylabel('Actual chlorophyll-a (µg/L)', 'FontSize', 20);
title('DockEye Calibration Curve', 'FontSize', 20);
legend('Chlorophyll-a', 'Regression');
set(gca, 'FontSize', 20); 
grid on;
box on;

subplot(2,1,2)
plot(xh2, y2, 'wo', 'MarkerSize', 6); hold on;
plot(xhfit, yhfit, 'b-', 'LineWidth', 2, 'DisplayName', 'Fit');
xlabel('Measured counts', 'FontSize', 20);
ylabel('Actual chlorophyll-a (µg/L)', 'FontSize', 20);
title('Horiba Calibration Curve', 'FontSize', 20);
legend('Chlorophyll-a', 'Regression');
set(gca, 'FontSize', 20); 
grid on;
box on;

% Add regression equation and R² to plot
eqn = sprintf('y = %.4fx + %.4f\nR² = %.4f', slope, intercept, r2);
text(0, 235, eqn, 'FontSize', 18);

eqn2 = sprintf('y = %.4fx + %.4f\nR² = %.4f', slope2, intercept2, r22);
text(0, 88, eqn2, 'FontSize', 18);
