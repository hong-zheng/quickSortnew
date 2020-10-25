#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

// 性能一般是O(nlogn)
// 时间复杂度:平均：O(nlogn) 最好 O(nlogn) 最坏时间复杂度：O(n^2)
// 空间复杂度：调用栈，空间不一定是变量个数，还包含栈空间  平均划分深度O(logn)
// 数据敏感
// 不稳定
// 数据有序可能栈溢出：有序的情况就会造成单边划分

// 三数取中:先用中数和头或者尾比较
int getMid(int* arr, int beg, int end){
	int mid = beg + (end - beg) / 2;
	// beg , mid , end 选择中间值的位置
	if (arr[beg] < arr[mid]){
		// beg < mid
		if (arr[mid] < arr[end])
			return mid;
		else
			// beg < mid , end <= mid
		{
			if (arr[beg] > arr[end]){
				return beg;
			}
			else{
				return end;
			}
		}
	}
	else{
		// beg > mid 

		// beg > mid , mid > end
		if (arr[mid] > arr[end]){
			return mid; 
		}
		else{
			// end >= mid
			if (arr[beg] > arr[end]){
				return end;
			}
			else{
				return beg;
			}
		}
	}
}
// hora划分
int ph(int* arr, int beg, int end){
	int key = arr[beg];
	int start = beg;
	while (beg < end){
		while (beg < end && arr[end] >= key)
			--end;
		while (beg < end && arr[beg] <= key)
			++beg;
		if (beg != end){
			swap(arr[beg], arr[end]);
		}
	}
	swap(arr[start], arr[beg]);
	return beg;
}

// hora划分
int phM(int* arr, int beg, int end){
	// 每次先做三数取中
	int mid = getMid(arr, beg, end);
	// 交换中间值和起始位置的值交换
	// mid <--> beg
	swap(arr[beg], arr[mid]);

	int key = arr[beg];
	int start = beg;
	while (beg < end){
		while (beg < end && arr[end] >= key)
			--end;
		while (beg < end && arr[beg] <= key)
			++beg;
		if (beg != end){
			swap(arr[beg], arr[end]);
		}
	}
	swap(arr[start], arr[beg]);
	return beg;
}

// 挖坑法
int pwf(int* arr, int beg, int end){

	// 每次先做三数取中
	int mid = getMid(arr, beg, end);
	// 交换中间值和起始位置的值交换
	// mid <--> beg
	swap(arr[beg], arr[mid]);

	int start = beg;
	int key = arr[beg];
	while (beg < end){
		while (beg < end && arr[end] >= key){
			--end;
		}
		arr[beg] = arr[end];
		while (beg < end && arr[beg] <= key){
			++beg;
		}
		arr[end] = arr[beg];
	}
	arr[beg] = key;
	return beg;
}

int zzhf(int* arr, int beg, int end){
	// 每次先做三数取中
	int mid = getMid(arr, beg, end);
	// 交换中间值和起始位置的值交换
	// mid <--> beg
	swap(arr[beg], arr[mid]);

	int key = arr[beg];
	int prev = beg;
	int start = beg;
	int cur = prev + 1;
	while (cur <= end){
		if (arr[cur] < key && ++prev != cur){
			swap(arr[cur], arr[prev]);
		}
		++cur;
	}
	swap(arr[prev], arr[start]);
	return prev;
}


void qs(int* arr, int beg, int end){
	if (beg > end) return;
	int keyPos = zzhf(arr, beg, end);
	qs(arr, beg, keyPos - 1);
	qs(arr, keyPos + 1, end);
}

#include <stack>
void qNoR(int* arr, int beg, int end){
	stack<int> st;
	st.push(beg);
	st.push(end);

	while (!st.empty()){
		int end = st.top();
		st.pop();
		int beg = st.top();
		st.pop();
		int keyPos = pwf(arr, beg, end);
		if (beg< keyPos-1){
			st.push(beg);
			st.push(keyPos - 1);
		}
		if (keyPos + 1 < end){
			st.push(keyPos + 1);
			st.push(end);
		}
	}
}

// 用队列和栈实现快排都是为了保存其区间
// 在当前区间中进行划分
// 划分完成之后再划分其子区间

// 用栈的非递归思想实现快速排序
void quickSortNoR(int* arr, int len){
	stack<int> st;
	if (len > 1){
		// 起始区间入栈：先右后左
		st.push(len-1);
		st.push(0);
	}
	// 遍历栈：划分栈中的每一个区间
	// 循环结束表示每个区间都划分完成
	while (!st.empty()){
		// 获取栈顶区间
		int beg = st.top();
		st.pop();
		int end = st.top();
		st.pop();
		// 划分
		int keyPos = pwf(arr, beg, end);
		// 子区间入栈：先入右区间
		// 右边有效： keyPos+1 , end
		if (keyPos + 1 < end){
			st.push(end);
			st.push(keyPos + 1);
		}
		// 左区间有效： beg ， keyPos-1
		if (beg < keyPos - 1){
			st.push(keyPos - 1);
			st.push(beg);
		}
	}
}

// 用队列实现快排
#include <queue>
void quickQueue(int* arr, int len){
	queue<int> q;
	if (len > 1){
		q.push(0);
		q.push(len - 1);
	}
	while (!q.empty()){
		int beg = q.front();
		q.pop();
		int end = q.front();
		q.pop();
		int keyPos = pwf(arr, beg, end);
		if (beg < keyPos - 1){
			q.push(beg);
			q.push(keyPos - 1);
		}
		if (keyPos + 1 < end){
			q.push(keyPos + 1);
			q.push(end);
		}
	}
}

//队列非递归：保存待划分的区间
void quickSortNoR2(int* arr, int len){
	queue<int> q;
	if (len > 1){
		// 先左后右
		q.push(0);
		q.push(len - 1);
	}
	while (!q.empty()){
		// 获取队头区间
		int beg = q.front();
		q.pop();
		int end = q.front();
		q.pop();
		// 划分
		int keyPos = pwf(arr, beg, end);
		// 子区间 入队
		if (beg < keyPos - 1){
			q.push(beg);
			q.push(keyPos - 1);
		}
		if (keyPos + 1 < end){
			q.push(keyPos + 1);
			q.push(end);
		}
	}
}

void merge(int* arr, int beg, int mid, int end){
	int beg1 = beg;
	int end1 = mid;
	int beg2 = mid + 1;
	int end2 = end;
	int* tmp = new int[end - beg + 2];
	int idx = 0;
	while (beg1 <= end1 && beg2 <= end2){
		if (arr[beg1] < arr[beg2]){
			tmp[idx++] = arr[beg1];
		}
		else{
			tmp[idx++] = arr[beg2];
		}
	}
	while (beg1 <= end1){
		tmp[idx++] = arr[beg1];
	}
	while (beg2 <= end2){
		tmp[idx++] = arr[beg2];
	}
	idx = 0;
	while (beg <= end){
		arr[beg++] = tmp[idx++];
	}
}


// 合并：需要知道两个有序子序列的区间
// [beg,mid] [mid+1,end]
void Merge(int* arr, int beg, int mid, int end, int* tmp){
	int beg1 = beg;
	int end1 = mid;
	int beg2 = mid + 1;
	int end2 = end;
	int idx = beg;
	// 两个区间都有值的时候进行合并
	while (beg1 <= end1 && beg2 <= end2){
		if (arr[beg1] < arr[beg2]){
			tmp[idx++] = arr[beg1++];
		}
		else{
			tmp[idx++] = arr[beg2++];
		}
	}
	if (beg1 <= end1){
		memcpy(tmp + idx, arr + beg1, sizeof(int)*(end1 - beg1 + 1));
	}
	if (beg2 <= end2){
		memcpy(tmp + idx, arr + beg2, sizeof(int)*(end2 - beg2 + 1));
	}

	// 拷贝到原始空间
	memcpy(arr + beg, tmp + beg, sizeof(int)*(end - beg + 1));
}


void ms(int* arr, int beg, int end,int* tmp){
	if (beg >= end) return;
	int mid = beg + (end - beg) / 2;

	// 首先保证子区间有序，子区间排序
	ms(arr, beg, mid,tmp);
	ms(arr, mid+1, end,tmp);
	// 合并有序子区间
	Merge(arr, beg, mid, end,tmp);
}

void test(){
	int arr[] = { 329, 0, 7, 9, 85, 61, 0, 98, 7, 497, 1 };
	int len = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < len; ++i){
		cout << arr[i] << " ";
	}
	cout << endl;
	int* tmp = new int[len];
	ms(arr, 0,len-1,tmp);
	for (int i = 0; i < len; ++i){
		cout << arr[i] << " ";
	}
	cout << endl;
}
int main(){
	test();
	//	tb();
	// t4();
	system("pause");
	return 0;
}


// 三数取中
int getMiddle(int* arr, int beg, int end){
	int mid = beg + (end - beg) / 2;
	if (arr[beg] > arr[mid]){
		// beg > mid
		if (arr[mid] > arr[end]){
			return mid;
		}
		// beg > mid  , mid < end
		else{
			if (arr[beg] > arr[end]){
				return end;
			}
			else{
				return beg;
			}
		}
	}
	else{
		// beg < mid
		if (arr[beg] > arr[end]){
			return end;
		}
		else{
			// beg < mid , beg <= end
			if (arr[end] > arr[mid]){
				return mid;
			}
			else{
				return end;
			}
		}
	}
}

void tst(){
	
}

int maiedn(){

	tst();

	system("pause");
	return 0;
}

/*
int partion1(int* arr, int beg, int end){
int key = arr[beg];
int start = beg;
while (beg < end){
while (beg < end && arr[end] >= key){
--end;
}
arr[beg] = arr[end];
while (beg < end && arr[beg] <= key){
++beg;
}
arr[end] = arr[beg];
}
arr[beg] = key;
return beg;
}
/*
//第二种划分方法：挖坑法
int partion2(int* arr, int beg, int end){
int key = arr[beg];
while (beg < end){
// 从后向前找
while (beg < end && arr[end] >= key){
--end;
}
arr[beg] = arr[end];
while (beg < end && arr[beg] < key)
++beg;
arr[end] = arr[beg];
}
// 最后一个坑：相遇的位置 --> 填基准值
arr[beg] = key;
// 返回基准值的位置
return beg;
}
*/

/*
int partion22(int* arr, int beg, int end){
	// 选择基准值
	int key = arr[beg];
	int start = beg;
	// 划分
	while (beg < end){
		// 从后往前找第一个小于 key 的位置
		while (beg < end && arr[end] >= key){
			--end;
		}
		// 从前向后找第一个大于key的位置
		while (beg < end && key >= arr[beg])
		{
			++beg;
		}
		// 交换 end , beg位置的值
		if (beg != end)
			swap(arr[beg], arr[end]);
	}
	// key 和相遇位置的数据交换
	swap(arr[beg], arr[start]);
	return beg;
}

int partion2(int* arr, int beg, int end){
	int start = beg;
	int key = arr[beg];
	while (beg < end){
		while (beg < end && arr[end] >= key){
			--end;
		}
		while (beg < end && arr[beg] <= key){
			++beg;
		}
		if (beg != end){
			swap(arr[beg], arr[end]);
		}
	}
	swap(arr[beg], arr[start]);
	return beg;
}

int partion3(int* arr, int beg, int end){
	int key = arr[beg];
	// prev ：已划分最后一个小于 key 值位置
	// cur : 下一个待划分小于 key 的位置
	int prev = beg;
	int cur = prev + 1;
	while (cur <= end){
		// cur : 下一个小于key值的位置，如果prev与cur连续则同时向后移动，否则就先交换再移动
		if (arr
		[cur] < key && ++prev != cur){
			swap(arr[cur], arr[prev]);
		}
		++cur;
	}
	// prev 肯定小于key值
	swap(arr[beg], arr[prev]);
	return prev;
}

// 第三种划分方式
// 前后指针法
// 数据被划分为两部分，大的向后挪动，小的向前挪动
int partion13(int* arr, int beg, int end){
	// 最后一个小于基准值的位置
	int prev = beg;
	//cur:新发现的下一个小于基准值的位置
	int cur = prev + 1;
	int key = arr[beg];
	while (cur <= end){
		// 新发现的小数据和尾指针指向的位置不连续，则中间含有大于基准值的数据
		// 故进行交换
		// 大数据向后挪，小数据向后挪
		if (arr[cur] < key && ++prev != cur)
			swap(arr[prev], arr[cur]);
		++cur;
	}
	swap(arr[beg], arr[prev]);
	return prev;
}

*/