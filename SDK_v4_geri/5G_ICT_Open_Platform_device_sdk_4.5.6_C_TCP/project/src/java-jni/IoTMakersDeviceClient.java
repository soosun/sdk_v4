package com.kt.iotmakers.device;

public class IoTMakersDeviceClient {


	public interface ControlCallBack {

		public abstract double onControlNumberData(String dev_id, String resource_name, String prop_name, double prop_value);
		public abstract String onControlStringData(String dev_id, String resource_name, String prop_name, String prop_value);
		public abstract int onControlIntegerData(String dev_id, String resource_name, String prop_name, int prop_value);
		public abstract double onControFloatData(String dev_id, String resource_name, String prop_name, double prop_value);
		public abstract boolean onControlBoolData(String dev_id, String resource_name, String prop_name, boolean prop_value);
		public abstract int onEndOfControl(IoTMakersDeviceClient client, String dev_id, String resource_name, long rep_body);
    }

	private ControlCallBack callback;



    public IoTMakersDeviceClient() {
        this.callback = null;
    }
 
    public void setCallBack(int hndl, ControlCallBack callback) {
        this.callback = callback;
		
		ImSetControlCallBack(hndl);
    }

	static {
		// export LD_LIBRARY_PATH=./
		//java -Djava.library.path=./jni-libs -classpath ./jni-libs/libiotmakers-std-device.jar:./jni-libs:./classes  Hello
		System.loadLibrary("iotmakers-std-device-jni");
	}
	
	public native void sayHello(String hello);
	public native int ImInit(int debug_level);                                                                                    
	public native int ImRelease(int hndl);                                                                                        
	public native int ImConnectTo(int hndl, String  ec_ip, int ec_port);                                                           
	public native int ImConnect(int hndl);                                                                                        
	public native int ImDisconnect(int hndl);                                                                                     
	public native int ImTurnCircuitBreakerOff(int hndl);                                                                          
	public native int ImTurnResponseWaitOff(int hndl);                                                                          
	public native int ImAuthDevice(int hndl, String im_dev_id, String im_dev_pw, String im_dev_gw);                                 
	public native int ImPoll(int hndl);                                                                                           
	public native int ImMSleep(int hndl, int msec);
	public native int ImSendData(int hndl, String json_str, String resource_name);                                                                
	public native int ImSendDataOnDev(int hndl, String json_str, String resource_name, String dev_id);                                             
	private native int ImSetControlCallBack(int hndl);                                                                                    
	//public native int ImSetReportForControl(long rep_body, String resource_name, String json_str);                                                                                    
	public native int ImSetReportDataOnDev(long rep_body, String json_str, String resource_name, String dev_id);                                                                                   

	public double onControlNumberData(String dev_id, String resource_name, String prop_name, double prop_value){
		if ( callback != null )		{
			return this.callback.onControlNumberData(dev_id, resource_name, prop_name, prop_value);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return (double)0;
	}

	public String onControlStringData(String dev_id, String resource_name, String prop_name, String prop_value){
		if ( callback != null )		{
			return this.callback.onControlStringData(dev_id, resource_name, prop_name, prop_value);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return null;
	}

	public int onControlIntegerData(String dev_id, String resource_name, String prop_name, int prop_value){
		if ( callback != null )		{
			return this.callback.onControlIntegerData(dev_id, resource_name, prop_name, prop_value);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0;
	}

	public double onControFloatData(String dev_id, String resource_name, String prop_name, double prop_value){
		if ( callback != null )		{
			return this.callback.onControFloatData(dev_id, resource_name, prop_name, prop_value);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0.0;
	}

	public boolean onControlBoolData(String dev_id, String resource_name, String prop_name, boolean prop_value){
		if ( callback != null )		{
			return this.callback.onControlBoolData(dev_id, resource_name, prop_name, prop_value);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return false;
	}

	public int onEndOfControl(String dev_id, String resource_name, long rep_body){
		if ( callback != null )		{
			return this.callback.onEndOfControl(this, dev_id, resource_name, rep_body);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0;
	}




	/* for TEST

	public static int square(int input){
		int output = input * input;
		return output;
	}

	public static void helloCallBackTest(int input){
		System.out.println("helloCallBackTest input = " + input);
	}

	public void helloCallBackTest(String hello){
		System.out.println("helloCallBackTest say = " + hello);
	}
	*/
}


