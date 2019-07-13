package cn.bluedrum;
import com.tencent.bugly.Bugly;
import android.util.Log;

public class BDQApplication extends org.qtproject.qt5.android.bindings.QtApplication{

    @Override
        public void onCreate() {
            super.onCreate();

              Log.d("BDQ","show front activity ");
              Bugly.init(getApplicationContext(), "7b422b4b66", true);
             //CrashReport.initCrashReport(getApplicationContext(), "7b422b4b66", true);

        }

}
