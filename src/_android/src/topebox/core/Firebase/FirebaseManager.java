package topebox.core.Firebase;


import android.app.Activity;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.util.Log;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import topebox.core.Actions.Action;
import topebox.core.Actions.ActionArg;
import topebox.core.Actions.ActionFacebookPostScreenArg;
import topebox.core.Actions.ActionFirebaseDownloadFile;
import topebox.core.Actions.ActionFirebaseDownloadFileArg;
import topebox.core.AppConfig;
import topebox.core.AppContext;

import static topebox.core.OsFunctions.GetDataPath;

public class FirebaseManager
{
    public static String mFirebaseConfigName = "android.config";
    public static byte[] data;
	public static FirebaseServices currentFirebaseInstance;
	public static Activity currentActivity;
    static ArrayList<ActionArg> _waitting_arg = new ArrayList<ActionArg>();

	public static void init(Activity activity)
	{
		currentActivity = activity;
		currentFirebaseInstance = new FirebaseServices(activity);

		if (AppConfig.UseFirebaseAnalystic)
		{
			currentFirebaseInstance.initAnalytics();
		}
		if (AppConfig.UseFirebaseRemoteConfig)
		{
			Log.i("DEBUG", "init => UseFirebaseRemoteConfig");
			currentFirebaseInstance.initRemoteConfig();
		}
	}

	public static boolean FirebaseAnalyticsLogEvent(String itemId, String itemName, String itemType, String itemValue)
	{
		if (AppConfig.UseFirebaseAnalystic)
		{
			return currentFirebaseInstance.logEvent(itemId, itemName, itemType, itemValue);
		}
		else
		{
			Log.i("DEBUG", "Firebase analytics is on turn off! see string.xml");
			return false;
		}
	}

	public static boolean FirebaseAnalyticsSetUserProperty(String propertyId, String value)
	{
		if (AppConfig.UseFirebaseAnalystic)
		{
			return currentFirebaseInstance.setUserProperty(propertyId, value);
		}
		else
		{
			Log.i("DEBUG", "Firebase analytics is on turn off! see string.xml");
			return false;
		}
	}

	// Remote config
	public static boolean FireBaseRemoteConfigGetBoolean(String strId, boolean default_value)
	{
		if (AppConfig.UseFirebaseRemoteConfig)
		{
			return currentFirebaseInstance.GetBoolean(strId, default_value);
		}
		else
		{
			Log.i("DEBUG", "Firebase Remote config is on turn off! see string.xml");
			return default_value;
		}
	}

	public static long FireBaseRemoteConfigGetLong(String strId, long default_value)
	{
		if (AppConfig.UseFirebaseRemoteConfig)
		{
			return currentFirebaseInstance.GetLong(strId, default_value);
		}
		else
		{
			Log.i("DEBUG", "Firebase Remote config is on turn off! see string.xml");
			return default_value;
		}
	}

	public static String FireBaseRemoteConfigGetString(String strId, String default_value)
	{
		if (AppConfig.UseFirebaseRemoteConfig)
		{
			return currentFirebaseInstance.GetString(strId, default_value);
		}
		else
		{
			Log.i("DEBUG", "Firebase Remote config is on turn off! see string.xml");
			return default_value;
		}
	}
	
	public static class DownloadFileTask extends AsyncTask<String, Void, Void> {
		@Override
		protected Void doInBackground(String... uids) {
			final String request_url = uids[0];//arg.requesUrl;
			final String butket_url = uids[1];//arg.butketUrl;
			final String localSaveFile = uids[2];//arg.localSaveFile;

			Log.i("DEBUG", "downloadFileFirebase request_url: " + request_url);
			Log.i("DEBUG", "downloadFileFirebase butket_url: " + butket_url);
			Log.i("DEBUG", "downloadFileFirebase localSaveFile: " + localSaveFile);

			final FirebaseStorage storage = FirebaseStorage.getInstance(butket_url);
			// Create a storage reference from our app
			final StorageReference storageRef = storage.getReference();
			Log.i("DEBUG", "downloadFileFirebase storageRef: " + storageRef);
			// Create a reference from an HTTPS URL
			// Note that in the URL, characters are URL escaped!
			final StorageReference httpsReference = storage.getReferenceFromUrl(request_url);
			httpsReference.getBytes(Long.MAX_VALUE).addOnSuccessListener(new OnSuccessListener<byte[]>() {
				@Override
				public void onSuccess(byte[] bytes) {
					// Use the bytes to display the image
					mFirebaseConfigName = localSaveFile;
					String dataPath = "data/data/" + GetDataPath();
					File file = new File(dataPath, mFirebaseConfigName);
					Log.i("DEBUG", "Load local app data from: " + file.getAbsoluteFile());
					OutputStream fOut = null;
					//if (!file.exists()) {
					try {
						int length = (int) file.length();
						fOut = new FileOutputStream(file);
						if (fOut != null) {
							Log.i("DEBUG", "Length file: " + bytes.length);
							for (int x = 0; x < bytes.length; x++) {
								//Log.i("DEBUG", "Log byte ======================" + bytes[x]);
								fOut.write(bytes[x]); // writes the bytes
							}
							fOut.close();
							Log.i("DEBUG", "Get appdata saved file done. Data length = " + length);
						}
					} catch (Exception ex) {
						Log.i("DEBUG", "Cannot get appdata saved file");
					}
					//}
					//else
					//{
					//Log.i("DEBUG", "Appdata save file exists!");
					//}
					Log.i("DEBUG", "===>>Download file success" + bytes.toString());
					ActionArg arg = _waitting_arg.get(0);
					arg.onDone();
					_waitting_arg.clear();
				}
			}).addOnFailureListener(new OnFailureListener() {
				@Override
				public void onFailure(@NonNull Exception exception) {
					// Handle any errors
					Log.i("DEBUG", "Download file failed");
					ActionArg arg = _waitting_arg.get(0);
					arg.onCancel();
					_waitting_arg.clear();
				}
			});
			return null;
		}
	}
	
	
	public static void downloadFileFirebase(ActionFirebaseDownloadFileArg arg) {
		_waitting_arg.add(arg);
		final String request_url = arg.requesUrl;
		final String butket_url = arg.butketUrl;
		final String localSaveFile = arg.localSaveFile;
		DownloadFileTask download = new DownloadFileTask();
		download.execute(request_url, butket_url, localSaveFile);
	}
}