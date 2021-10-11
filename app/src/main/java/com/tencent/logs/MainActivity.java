package com.tencent.logs;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.TextView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-log");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        requestPermision();
    }

    private void doInit() {
        File file = new File(Environment.getExternalStorageDirectory(), "test.log");
        FileLogger fileLogger = new FileLogger(file, 20 * 1024 * 1024);
        long start = System.currentTimeMillis();
        for (int i=0;i<1000;i++){
            fileLogger.write("name:xxx0->");
        }
        long end = System.currentTimeMillis();
        Log.e("TAG","fileLogger->"+(end - start));

        SharedPreferences sp = getSharedPreferences("name",MODE_PRIVATE);
        SharedPreferences.Editor editor = sp.edit();
        start = System.currentTimeMillis();
        for (int i=0;i<1000;i++){
            editor.putString("name","xxx0->");
            editor.commit();
        }
        end = System.currentTimeMillis();
        Log.e("TAG","SharedPreferences->"+(end - start));

        File file2 = new File(Environment.getExternalStorageDirectory(), "test2.log");
        start = System.currentTimeMillis();
        try {
            FileOutputStream fileOutputStream=new FileOutputStream(file2);
            fileOutputStream.write("name:xxx0->".getBytes());
            end=System.currentTimeMillis();
            Log.e("TAG","fileOutputStream->"+(end - start));
            fileOutputStream.flush();
//            fileOutputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //读写权限
    private static String[] PERMISSIONS_STORAGE = {
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE,
//            Manifest.permission.RECORD_AUDIO,
    };
    //请求状态码
    private static int REQUEST_PERMISSION_CODE = 1;
    public void requestPermision() {
//        Log.e(TAG,"requestPermision");
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.LOLLIPOP) {
//            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
//                ActivityCompat.requestPermissions(this, PERMISSIONS_STORAGE, REQUEST_PERMISSION_CODE);
//                return;
//            }
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, PERMISSIONS_STORAGE, REQUEST_PERMISSION_CODE);
                return;
            }
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, PERMISSIONS_STORAGE, REQUEST_PERMISSION_CODE);
                return;
            }
        }
        doInit();
    }
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_PERMISSION_CODE) {
            boolean granted=true;
            for (int i = 0; i < permissions.length; i++) {
                Log.i("MainActivity", "申请的权限为：" + permissions[i] + ",申请结果：" +
                        grantResults[i]);
                if(grantResults[i]!=PackageManager.PERMISSION_GRANTED){
                    granted=false;
                    break;
                }
            }
            if(granted){
                doInit();
            }
        }
    }

}
