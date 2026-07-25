import { computed, Injectable, signal } from '@angular/core';
import { formatUnixSeconds } from '../../shared/utils/time.utils';

@Injectable({ providedIn: 'root' })
export class DeviceClockService {
  private readonly baseDeviceTimeSeconds = signal(0);
  private readonly baseBrowserTimeMs = signal(Date.now());
  private readonly currentBrowserTimeMs = signal(Date.now());

  readonly currentDeviceTimeSeconds = computed(() => {
    const base = this.baseDeviceTimeSeconds();
    if (base <= 0) return 0;

    const elapsedSeconds = Math.floor((this.currentBrowserTimeMs() - this.baseBrowserTimeMs()) / 1000);
    return base + elapsedSeconds;
  });

  readonly timeLabel = computed(() => formatUnixSeconds(this.currentDeviceTimeSeconds()));

  constructor() {
    window.setInterval(() => {
      this.currentBrowserTimeMs.set(Date.now());
    }, 1000);
  }

  setDeviceTime(deviceTimeSeconds: number): void {
    if (!Number.isFinite(deviceTimeSeconds) || deviceTimeSeconds <= 0) return;

    this.baseDeviceTimeSeconds.set(deviceTimeSeconds);
    this.baseBrowserTimeMs.set(Date.now());
    this.currentBrowserTimeMs.set(Date.now());
  }
}
