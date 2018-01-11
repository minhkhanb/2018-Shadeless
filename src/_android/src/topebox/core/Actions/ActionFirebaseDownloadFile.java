//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;
import topebox.core.*;
import topebox.core.Firebase.FirebaseManager;

//#[001] import com.facebook.*;
//#[001] import com.facebook.model.*;

import 	android.util.Log;

public class ActionFirebaseDownloadFile implements Action
{

    ActionArg Arg = null;
    public ActionFirebaseDownloadFile( ActionArg arg)
    {
        Arg = arg;
    }

    public void act()
    {
        if(Arg == null )
        {
            Log.i("Debug", "action download config file error");
            return;
        }
        Log.i("DEBUG","begin do action download config file avatar");
        Arg.onBegin();
        FirebaseManager.downloadFileFirebase((ActionFirebaseDownloadFileArg) Arg);
    }
}