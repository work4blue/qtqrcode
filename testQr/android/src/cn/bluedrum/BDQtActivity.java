package cn.bluedrum;

import android.app.Activity;
import android.annotation.TargetApi;
import android.app.NotificationManager;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import org.qtproject.qt5.android.bindings.QtActivity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.Manifest;
import java.util.List;
//import android.support.v4.content.FileProvider;
import android.content.pm.PackageManager.NameNotFoundException;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import android.net.Uri;
import android.util.Log;
import android.os.Build;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;

import android.widget.Toast;
import android.content.ClipData;
import android.content.ClipboardManager;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.content.BroadcastReceiver;





public class BDQtActivity extends QtActivity
{
    public static final int SHARE_ACTIVITE_ID = 10002;
    public static final int CAMERA_ACTIVITE_ID = 10003;


    public static final int OS_STATE_FOREGROUND = 1;
    public static final int OS_STATE_SCREEN = 2;
    public static final int OS_STATE_UNLOCK = 3;
    //1 == 前台切换
    //2 == 亮屏切换
    //3 == 解锁


    boolean  mCreated = false;

    private final static String TAG = "AndroidOwlFit";
    @Override
    public void onCreate(Bundle savedInstanceState) {

        //   initStatusBar();
        try{
            super.onCreate(savedInstanceState);
        }
        catch(Exception e){
        }







        //shareFile2("share...","/sdcard/Download/2.jpg");

        mCreated = true;

    }

    @Override
    protected void onStop() {

        super.onStop();
        Log.v("Owlfit", "程序进入后台");


    }

    @Override
    protected void onStart() {
        //TODO Auto-generated method stub
        super.onStart();
        Log.v("Owlfit", "程序进入前台");     
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

    }





    private static final int PERMISSION_REQUEST_COARSE_LOCATION = 0xb01;
    private static final int PERMISSION_REQUEST_CAMERA = 0xb02;
    public static boolean requestBlePermission(Activity activity,int requestId){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

            //必须打开定位服务,才能正常工作

            //Android 6.0 以上版本必须等定位申请后,startScan 才有数据,提前扫描因无权限,将收不到数据
            if (activity.checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                activity.requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, requestId);
                return false;
            }

            if (activity.checkSelfPermission(Manifest.permission.BLUETOOTH_ADMIN ) != PackageManager.PERMISSION_GRANTED) {
                activity.requestPermissions(new String[]{Manifest.permission.BLUETOOTH_ADMIN}, requestId);
                return false;
            }


            if (activity.checkSelfPermission(Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                activity.requestPermissions(new String[]{Manifest.permission.CAMERA}, PERMISSION_REQUEST_CAMERA);
            }


        }

        return true;
    }





    boolean checkFilePermission(){
        if (Build.VERSION.SDK_INT >= 23) {

            int REQUEST_CODE_CONTACT = 101;
            String[] permissions = {Manifest.permission.WRITE_EXTERNAL_STORAGE};
            //验证是否许可权限
            for (String str : permissions) {
                if (this.checkSelfPermission(str) != PackageManager.PERMISSION_GRANTED)
                {  //申请权限
                    this.requestPermissions(permissions, REQUEST_CODE_CONTACT);
                    return true;
                }

            }
        }

        return false;
    }


//    static void startCamera(Activity context,boolean take){
//        CommUtils.startActivityForResult(context,CAMERA_ACTIVITE_ID,CameraActivity.class,take?1:0);
//    }


//    @Override
//    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
//        if(requestCode == CAMERA_ACTIVITE_ID){
//            AndroidJniObj.exitCamera();
//        }

//    }




}
