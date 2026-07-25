import { computed, inject, Injectable, signal } from '@angular/core';
import { WifiSetupMode } from '../../core/models/settings.models';
import { DeviceClockService } from '../../core/services/device-clock.service';
import { RelayApiService } from '../../core/services/relay-api.service';
import { SettingsApiService } from '../../core/services/settings-api.service';
import { toUnixSeconds } from '../../shared/utils/time.utils';

@Injectable()
export class SettingsVm {
  private readonly deviceClock = inject(DeviceClockService);

  readonly mode = signal<WifiSetupMode>('ap');
  readonly ssid = signal('');
  readonly password = signal('');
  readonly telegramToken = signal('');
  readonly savingWifi = signal(false);
  readonly savingToken = signal(false);
  readonly syncingTime = signal(false);
  readonly error = signal('');
  readonly message = signal('');

  readonly deviceTimeLabel = this.deviceClock.timeLabel;

  constructor(
    private readonly settingsApi: SettingsApiService,
    private readonly relayApi: RelayApiService,
  ) {}

  async load(): Promise<void> {
    try {
      const status = await this.relayApi.getStatus();
      this.deviceClock.setDeviceTime(status.time);
    } catch {
      this.error.set('Device time failed to load.');
    }
  }

  async saveWifi(): Promise<void> {
    this.clearFeedback();

    if (this.mode() === 'station' && !this.ssid().trim()) {
      this.error.set('SSID is required for WiFi mode.');
      return;
    }

    this.savingWifi.set(true);
    try {
      if (this.mode() === 'ap') {
        await this.settingsApi.setupAccessPoint();
        this.message.set('AP mode saved.');
      } else {
        await this.settingsApi.setupWifi({
          mode: 'station',
          ssid: this.ssid(),
          pass: this.password(),
        });
        this.message.set('WiFi settings saved.');
      }
    } catch {
      this.error.set('WiFi settings failed to save.');
    } finally {
      this.savingWifi.set(false);
    }
  }

  async saveTelegramToken(): Promise<void> {
    this.clearFeedback();
    this.savingToken.set(true);

    try {
      await this.settingsApi.saveTelegramToken({ token: this.telegramToken() });
      this.telegramToken.set(this.telegramToken().trim());
      this.message.set('Telegram token saved.');
    } catch {
      this.error.set('Telegram token failed to save.');
    } finally {
      this.savingToken.set(false);
    }
  }

  async syncTime(): Promise<void> {
    this.clearFeedback();
    this.syncingTime.set(true);

    try {
      await this.settingsApi.syncTime(toUnixSeconds(new Date()));

      await this.load();
      this.message.set('Device time synchronized.');
    } catch {
      this.error.set('Device time failed to synchronize.');
    } finally {
      this.syncingTime.set(false);
    }
  }

  private clearFeedback(): void {
    this.error.set('');
    this.message.set('');
  }
}
