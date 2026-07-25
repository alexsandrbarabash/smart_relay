import { computed, inject, Injectable, signal } from '@angular/core';
import { RelayStatus, RelayTab, TimerUnit } from '../../core/models/relay.models';
import { DeviceClockService } from '../../core/services/device-clock.service';
import { RelayApiService } from '../../core/services/relay-api.service';
import {
  addDurationToNow,
  localDateTimeToUnixSeconds,
  nextHourTimeInput,
  todayDateInput,
  toUnixSeconds,
} from '../../shared/utils/time.utils';

@Injectable()
export class RelayVm {
  private readonly deviceClock = inject(DeviceClockService);

  readonly tabs: { id: RelayTab; label: string }[] = [
    { id: 'power', label: 'Power' },
    { id: 'timer', label: 'Timer' },
    { id: 'schedule', label: 'Schedule' },
  ];

  readonly activeTab = signal<RelayTab>('power');
  readonly status = signal<RelayStatus>({
    deviceName: 'Smart Relay',
    relayState: 'off',
    physicalState: false,
    telegramActive: false,
    time: 0,
  });
  readonly powerActionOn = signal(true);
  readonly timerAmount = signal(15);
  readonly timerUnit = signal<TimerUnit>('minutes');
  readonly timerActionOn = signal(true);
  readonly scheduleDate = signal(todayDateInput());
  readonly scheduleTime = signal(nextHourTimeInput());
  readonly scheduleActionOn = signal(true);
  readonly saving = signal(false);
  readonly error = signal('');
  readonly message = signal('');

  readonly physicalStatusLabel = computed(() => (this.status().physicalState ? 'Relay On' : 'Relay Off'));

  readonly controllerStatusLabel = computed(() =>
    this.status().relayState === 'scheduled' ? 'Scheduled' : 'Normal',
  );

  readonly telegramStatusLabel = computed(() =>
    this.status().telegramActive ? 'Bot Active' : 'Bot Inactive',
  );
  readonly deviceTimeLabel = this.deviceClock.timeLabel;

  constructor(
    private readonly relayApi: RelayApiService,
  ) {}

  async load(): Promise<void> {
    const status = await this.relayApi.getStatus();
    this.status.set(status);
    this.deviceClock.setDeviceTime(status.time);
    this.powerActionOn.set(this.status().physicalState);
  }

  selectTab(tab: RelayTab): void {
    this.activeTab.set(tab);
    this.clearFeedback();
  }

  async savePower(): Promise<void> {
    await this.runCommand(async () => {
      const nextStatus = this.powerActionOn()
        ? await this.relayApi.turnOn()
        : await this.relayApi.turnOff();

      this.status.set(nextStatus);
      this.message.set(`Relay ${this.powerActionOn() ? 'enabled' : 'disabled'}.`);
    });
  }

  async saveTimer(): Promise<void> {
    const amount = Number(this.timerAmount());
    if (!Number.isFinite(amount) || amount <= 0) {
      this.error.set('Timer duration must be greater than zero.');
      return;
    }

    const time = addDurationToNow(amount, this.timerUnit());
    await this.schedule(time, this.timerActionOn(), 'Timer scheduled.');
  }

  async saveSchedule(): Promise<void> {
    if (!this.scheduleDate() || !this.scheduleTime()) {
      this.error.set('Date and time are required.');
      return;
    }

    const time = localDateTimeToUnixSeconds(this.scheduleDate(), this.scheduleTime());
    if (time <= toUnixSeconds(new Date())) {
      this.error.set('Schedule time must be in the future.');
      return;
    }

    await this.schedule(time, this.scheduleActionOn(), 'Schedule saved.');
  }

  private async schedule(time: number, targetActionOn: boolean, successMessage: string): Promise<void> {
    await this.runCommand(async () => {
      const nextStatus = await this.relayApi.scheduleAction({
        operation: targetActionOn ? 1 : 0,
        time,
      });

      this.status.set(nextStatus);
      this.message.set(successMessage);
    });
  }

  private async runCommand(command: () => Promise<void>): Promise<void> {
    this.saving.set(true);
    this.clearFeedback();

    try {
      await command();
    } catch {
      this.error.set('Command failed. Try again.');
    } finally {
      this.saving.set(false);
    }
  }

  private clearFeedback(): void {
    this.error.set('');
    this.message.set('');
  }
}
