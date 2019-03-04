#define WIFI_NAME "$$?id: wifiName type: string name:wifi name hint: Your wifi name$$"
#define WIFI_PASSWORD "$$?id: wifiPassword type:password name: password hint: Your wifi password$$"
#define DEVICE_ID $$esp1$$
#define DEVICE_NAME "$$esp1_name$$"
#define TOKEN "$$id: token type: token pos: -1000$$"
maciek sajdak


$$id: active type:boolean name: is active hint: is active will be also active2 default: 1$$
$$IF active == 1$$
	$$id: active2 type:boolean name: is active 2 hint:wlaczony jak active ON$$
$$/IF$$

$$IF active != 1$$
	$$id: active3 type:boolean name: is active 2 hint:wlaczony jak active OFF$$
$$/IF$$

$$IF active == 1$$
$$ELSE$$
	$$id: active4 type:boolean name: is active 2 hint:wlaczony jak active OFF$$
$$/IF$$


$$IF active == 1$$
	$$IF active2 == 1$$
		$$id: active5 type:boolean name: is active 2 hint:wlaczony jak active i active 2$$
	$$/IF$$
$$/IF$$