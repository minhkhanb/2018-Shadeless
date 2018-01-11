package topebox.core.Tapdaq;

import android.util.Log;

import com.tapdaq.sdk.common.TMAdError;
import com.tapdaq.sdk.listeners.TMAdListener;

import topebox.core.TapdaqManager;

public class InterstitialListener extends TMAdListener {

    @Override
    public void didLoad() {
        Log.i("DEBUG", "Java tapdaq interstitial callback load success!");
        TapdaqManager.onInterstitialLoaded(true);
    }

    @Override
    public void didFailToLoad(TMAdError error)
    {
        Log.i("DEBUG", "Java tapdaq interstitial callback load fail! error: " + error.getErrorMessage());
        TapdaqManager.onInterstitialLoaded(false);
    }

    @Override
    public void didClose() {
        Log.i("DEBUG","Java tapdaq interstitial callback Ads Closed");
        TapdaqManager.onInterstitialClose(true);
    }
}