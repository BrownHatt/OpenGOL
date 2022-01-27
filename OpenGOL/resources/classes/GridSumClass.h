#pragma once
class GridSumClass
{
public:


		short* sumArea;
		int sumAreaWidth;
		int sumAreaHeight;
		int sumAreaSize;
		int sumSizeWidth;

		GridSumClass(int width, int height, short* sumarea) {
			sumArea = sumarea;
			sumAreaWidth = width;
			sumAreaHeight = height;
			sumAreaSize = sumAreaHeight * sumAreaWidth;
			sumSizeWidth = 5;
		};
		GridSumClass() = default;

		void analyzeCellArea(short* cellArea, int areaSize, int areaWidth) {		
			int x = 0;
			int y = 0;	
			//cout << "Analyze: " << sumAreaSize << "steps";
			for (int i = 0; i < sumAreaSize - (sumAreaWidth * sumSizeWidth) - sumSizeWidth; i++) {
				sumArea[i] = getSum(x, y, cellArea, areaWidth);
				x += sumSizeWidth;
				if (x > areaWidth-sumSizeWidth) {
					x = 0;
					y += sumSizeWidth;
				}

			}
			//cout << "--------" << endl;
			//showSumArea();
		};

		int getSum(int x, int y, short* cellArea, int areaWidth) {
			int offset = 0;
			int sum = 0;
			//cout << "checking: " << x << "-" << y << ":" << endl;
			for (int i = 0; i < (sumSizeWidth * sumSizeWidth); i++) {
				//cout << cellArea[y * areaWidth + x + offset] << " ";
				sum += (short)cellArea[y * areaWidth + x + offset];
				offset += 1;
				if (offset == sumSizeWidth) {					
				//	cout << endl;
					y += 1;
					offset = 0;
				}
			}
			return sum;
		}

		void showSumArea() {
			for (int i = 0; i < sumAreaSize; i++)
			{
				if (i % sumAreaWidth == 0)
					cout << endl;
				cout << (float)(sumArea[i] / 25.0f) << "-";
			}
			cout << endl;
		}
};

