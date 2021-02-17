n = 1;
start = 9;
iterations = 9;
 datamean = transpose(1:1:36);
 datamean(1:1:36) = 0;
for n = start:(start+iterations)
    datamean = (datamean + DoseperParticle(((n-1)*36)+1:1:(n)*36));
end
datamean = datamean/iterations;