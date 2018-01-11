package topebox.core.Tapdaq;

import android.util.Log;

import com.tapdaq.sdk.common.TMAdError;
import com.tapdaq.sdk.listeners.TMAdListener;
import com.tapdaq.sdk.model.rewards.TDReward;

import java.util.Locale;

import topebox.core.TapdaqManager;

public class RewardedVideoListener extends TMAdListener {

    @Override
    public void didLoad() {
        Log.i("DEBUG", "Java tapdaq video callback load success!");
        TapdaqManager.onVideoLoaded(true);
    }

    @Override
    public void didFailToLoad(TMAdError error)
    {
        Log.i("DEBUG", "Java tapdaq video callback load fail! error: " + error.getErrorMessage());
        TapdaqManager.onVideoLoaded(false);
    }

    @Override
    public void didVerify(TDReward reward) {
        Log.i("MEDIATION-SAMPLE", String.format(Locale.ENGLISH, "didVerify: ID: %s, Tag: %s. Reward: %s. Value: %f. Valid: %b. Custom Json: %s", reward.getEventId(), reward.getTag(), reward.getName(), reward.getValue(), reward.isValid(), reward.getCustom_json().toString()));
        TapdaqManager.onVideoCompleted(true, false);
    }
}