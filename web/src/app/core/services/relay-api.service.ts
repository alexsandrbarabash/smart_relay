import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { firstValueFrom } from 'rxjs';
import { RelayStatus, ScheduleRelayCommand } from '../models/relay.models';

interface StatusResponse {
  deviceName: string;
  relayState: 'on' | 'off' | 'scheduled';
  physicalState: boolean;
  telegramActive: boolean;
  time?: number;
}

@Injectable({ providedIn: 'root' })
export class RelayApiService {
  constructor(private readonly http: HttpClient) {}

  async getStatus(): Promise<RelayStatus> {
    const response = await firstValueFrom(this.http.get<StatusResponse>('/status'));
    return this.normalizeStatus(response);
  }

  async turnOn(): Promise<RelayStatus> {
    await firstValueFrom(this.http.post('/on', null, { responseType: 'text' }));
    return this.getStatus();
  }

  async turnOff(): Promise<RelayStatus> {
    await firstValueFrom(this.http.post('/off', null, { responseType: 'text' }));
    return this.getStatus();
  }

  async scheduleAction(command: ScheduleRelayCommand): Promise<RelayStatus> {
    const headers = new HttpHeaders({
      'Content-Type': 'application/json',
    });

    await firstValueFrom(this.http.post('/timer', command, { headers }));
    return this.getStatus();
  }

  private normalizeStatus(response: StatusResponse): RelayStatus {
    return {
      deviceName: response.deviceName?.trim() || 'Smart Relay',
      relayState:
        response.relayState === 'on' || response.relayState === 'scheduled'
          ? response.relayState
          : 'off',
      physicalState: Boolean(response.physicalState),
      telegramActive: Boolean(response.telegramActive),
      time: Number.isFinite(response.time) ? Number(response.time) : 0,
    };
  }
}
