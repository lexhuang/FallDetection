package com.ntut.tof.falldetection;

import java.util.HashMap;
import java.util.Iterator;

import com.ntut.falldetection.R;

import android.app.Activity;
import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;


public class MainActivity extends Activity {
	private Button startDeviceButton;
	private LinearLayout mainLayout;
	private TOFManager manager; 
	
	static{
		System.loadLibrary("usb1.0");
		System.loadLibrary("tof");
		System.loadLibrary("FallDetection");
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        manager = new TOFManager();
        
        startDeviceButton = (Button)findViewById(R.id.startDeviceButton);
        startDeviceButton.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				TOFManager.startDevice();
			}
        });
        UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
        
        HashMap<String, UsbDevice> deviceList = manager.getDeviceList();
        Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
        while(deviceIterator.hasNext()){
            UsbDevice device = deviceIterator.next();
            
        }
    }
}
