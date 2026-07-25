import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { LucideSettings, LucidePlug } from '@lucide/angular';
import { AppShell } from '../../shared/components/app-shell/app-shell';
import { IconButton } from '../../shared/components/icon-button/icon-button';
import { StatusPill } from '../../shared/components/status-pill/status-pill';
import { Tabs } from '../../shared/components/tabs/tabs';
import { RelayVm } from './relay.vm';
import { PowerPanel } from './components/power-panel/power-panel';
import { TimerPanel } from './components/timer-panel/timer-panel';
import { SchedulePanel } from './components/schedule-panel/schedule-panel';

@Component({
  selector: 'app-relay-page',
  standalone: true,
  imports: [
    LucideSettings,
    LucidePlug,
    AppShell,
    IconButton,
    StatusPill,
    Tabs,
    PowerPanel,
    TimerPanel,
    SchedulePanel,
  ],
  providers: [RelayVm],
  templateUrl: './relay.page.html',
  styleUrl: './relay.page.scss',
})
export class RelayPage implements OnInit {
  constructor(
    protected readonly vm: RelayVm,
    private readonly router: Router,
  ) {}

  ngOnInit(): void {
    void this.vm.load();
  }

  openSettings(): void {
    void this.router.navigateByUrl('/settings');
  }
}
